#include <engine/core/engine.h>
#include <engine/network/host.h>
#include <engine/network/snapshot.h>
#include <engine/public/scene_service.h>
#include <engine/util/uuid.h>

#include <cstring>
#include <stdexcept>

Host::Host(std::reference_wrapper<Router> router, int connection_port,
           int max_clients)
    : server_{nullptr},
      connection_port_{connection_port},
      max_clients_{max_clients},
      connection_state_{ConnectionState::NONE},
      local_uuid_{uuid::generate_uuid_v4()},
      router_{router} {
  set_client_disconnect_handler();
  set_client_connect_handler();
}

Host::~Host() noexcept {
  if (server_) {
    enet_host_flush(server_);
    enet_host_destroy(server_);
    server_ = nullptr;
  }

  router_.get().unregister_handler(DefaultMessageTypes::CLIENT_DISCONNECT);
  router_.get().unregister_handler(DefaultMessageTypes::CONNECT);
}

void Host::start_server() {
  // Find local IP address
  ENetAddress addr{};
  addr.host = ENET_HOST_ANY;

  ENetHost* host = enet_host_create(&addr, 1, 2, 0, 0);
  if (!host) throw std::runtime_error("Failed to create ENet host");

  ENetAddress remote{};
  enet_address_set_host(&remote, "8.8.8.8");
  remote.port = 1;
  enet_socket_connect(host->socket, &remote);

  ENetAddress local{};
  enet_socket_get_address(host->socket, &local);

  char ip[32];
  enet_address_get_host_ip(&local, ip, sizeof(ip));
  ip_ = ip;
  enet_host_destroy(host);

  // Start server
  ENetAddress address{};
  address.host = ENET_HOST_ANY;
  address.port = connection_port_;

  size_t channel_limit = 2;
  enet_uint32 max_bandwidth = 0;

  server_ = enet_host_create(&address, max_clients_, channel_limit,
                             max_bandwidth, max_bandwidth);

  if (!server_) throw std::runtime_error("Failed to create ENet server host.");

  connection_state_ = ConnectionState::CONNECTED;
}

void Host::poll() noexcept {
  if (!server_) return;

  ENetEvent event{};

  while (enet_host_service(server_, &event, 0) > 0) {
    switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT: {
        // Reject connection if over limit
        if (clients_.size() >= static_cast<size_t>(max_clients_)) {
          enet_peer_disconnect(event.peer, 0);
          break;
        }

        // Assign UUID
        const std::string uuid = uuid::generate_uuid_v4();
        clients_.emplace(uuid, event.peer);

        MsgConnect body{};
        std::strncpy(body.uuid, uuid.c_str(), sizeof(body.uuid) - 1);

        Message msg = serialize_message(body, DefaultMessageTypes::CONNECT);

        // Send CONNECT message to the client
        send_to_peer(msg, event.peer);

        // Also route CONNECT through the router so the engine's snapshot
        // handler fires
        router_.get().route(msg);
      } break;

      case ENET_EVENT_TYPE_DISCONNECT:
        // The disconnect handler registered in Router handles cleanup logic.
        break;

      case ENET_EVENT_TYPE_RECEIVE: {
        Message msg;

        uint16_t raw_type;
        std::memcpy(&raw_type, event.packet->data, sizeof(raw_type));
        msg.header.type = MessageType::from_raw(raw_type);

        msg.header.size = event.packet->dataLength - sizeof(raw_type);

        msg.payload.assign(event.packet->data + sizeof(raw_type),
                           event.packet->data + event.packet->dataLength);

        router_.get().route(msg);

        enet_packet_destroy(event.packet);
      } break;

      default:
        break;
    }
  }
}

void Host::broadcast(const Message& message) noexcept {
  if (clients_.empty()) return;

  uint16_t raw_type = MessageType::to_raw(message.header.type);
  const size_t packet_size = sizeof(raw_type) + message.payload.size();

  ENetPacket* packet = enet_packet_create(nullptr, packet_size, 0);

  if (!packet) return;

  std::memcpy(packet->data, &raw_type, sizeof(raw_type));

  if (!message.payload.empty()) {
    std::memcpy(packet->data + sizeof(raw_type), message.payload.data(),
                message.payload.size());
  }

  // Send to all
  for (auto& [uuid, peer] : clients_) {
    enet_peer_send(peer, 0, packet);
  }
}

void Host::disconnect() noexcept {
  if (!server_) return;

  connection_state_ = ConnectionState::DISCONNECTING;

  // Notify clients
  MsgDisconnect body{};
  std::strncpy(body.uuid, local_uuid_.c_str(), sizeof(body.uuid) - 1);

  Message msg = serialize_message(body, DefaultMessageTypes::HOST_DISCONNECT);
  broadcast(msg);

  enet_host_flush(server_);

  // Disconnect all clients
  for (auto& [uuid, peer] : clients_) {
    if (peer) enet_peer_disconnect_later(peer, 0);
  }

  clients_.clear();

  // Destroy server
  enet_host_destroy(server_);
  server_ = nullptr;

  connection_state_ = ConnectionState::DISCONNECTED;
}

void Host::send_to_peer(const Message& message, ENetPeer* peer) noexcept {
  if (!peer) return;

  uint16_t raw_type = MessageType::to_raw(message.header.type);
  const size_t packet_size = sizeof(raw_type) + message.payload.size();

  ENetPacket* packet = enet_packet_create(nullptr, packet_size, 0);

  if (!packet) return;

  std::memcpy(packet->data, &raw_type, sizeof(raw_type));

  if (!message.payload.empty()) {
    std::memcpy(packet->data + sizeof(raw_type), message.payload.data(),
                message.payload.size());
  }

  enet_peer_send(peer, 0, packet);
}

void Host::send_to_peer_via_uuid(const std::string& uuid,
                                 const Message& message) noexcept {
  auto it = clients_.find(uuid);
  if (it == clients_.end()) return;

  send_to_peer(message, it->second);
}

void Host::sync() noexcept {
  const auto now = std::chrono::steady_clock::now();
  if (now - last_snapshot_time_ >= snapshot_interval_) {
    auto& engine = Engine::instance();
    auto& scene_service = engine.services->get_service<SceneService>().get();

    last_snapshot_time_ = now;
    Message delta = snapshot::create_delta_snapshot(
        scene_service.current_scene(), DefaultMessageTypes::SNAPSHOT_DELTA);
    // Only broadcast if there is payload beyond the count header
    if (delta.header.size > sizeof(uint16_t)) {
      broadcast(delta);
    }
  }
}

ConnectionState Host::get_connection_state() const noexcept {
  return connection_state_;
}

std::string Host::get_uuid() const noexcept { return local_uuid_; }

void Host::set_max_clients(int amount) noexcept { max_clients_ = amount; }

int Host::get_client_amount() const noexcept {
  return static_cast<int>(clients_.size());
}

std::string Host::get_ip() const noexcept { return ip_; }

void Host::set_connection_port(int port) noexcept { connection_port_ = port; }

void Host::set_client_disconnect_handler() noexcept {
  // Client disconnect handler
  auto handler = [this](const Message& message) {
    MsgConnect data{};
    std::memcpy(&data, message.payload.data(), sizeof(data));

    auto it = clients_.find(data.uuid);
    if (it != clients_.end()) {
      enet_peer_disconnect(it->second, 0);
      clients_.erase(it);
      // TODO: Broadcasting client disconnection to others if needed.
    }
  };

  router_.get().register_handler(DefaultMessageTypes::CLIENT_DISCONNECT,
                                 std::move(handler));
}

void Host::set_client_connect_handler() noexcept {
  auto handler = [this](const Message& msg) {
    // Extract UUID from the CONNECT message
    struct MsgConnect {
      char uuid[37];
    } data{};
    if (msg.payload.size() >= sizeof(data)) {
      std::memcpy(&data, msg.payload.data(), sizeof(data));
      auto& engine = Engine::instance();
      auto& scene_service = engine.services->get_service<SceneService>().get();

      Message snapshot_msg = snapshot::create_full_snapshot(
          scene_service.current_scene(), DefaultMessageTypes::SNAPSHOT_FULL);
      send_to_peer_via_uuid(std::string(data.uuid), snapshot_msg);
    }
  };

  router_.get().register_handler(DefaultMessageTypes::CONNECT,
                                 std::move(handler));
}
