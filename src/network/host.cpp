#include <engine/network/host.h>
#include <engine/util/uuid.h>

#include <cstring>
#include <stdexcept>

Host::Host(std::shared_ptr<Router> router, int connection_port, int max_clients)
    : server_{nullptr},
      connection_port_{connection_port},
      max_clients_{max_clients},
      connection_state_{ConnectionState::NONE},
      local_uuid_{uuid::generate_uuid_v4()},
      router_{std::move(router)}
{
    set_client_disconnect_handler();
}

Host::~Host() noexcept
{
    if (server_) {
        enet_host_flush(server_);
        enet_host_destroy(server_);
        server_ = nullptr;
    }
}

void Host::start_server()
{
    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = connection_port_;

    server_ = enet_host_create(&address, max_clients_, 2, 0, 0);

    if (!server_)
        throw std::runtime_error("Failed to create ENet server host.");

    connection_state_ = ConnectionState::CONNECTED;
}

void Host::poll() noexcept
{
    if (!server_)
        return;

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

            Message msg = SerializeMessage(body, DefaultMessageTypes::CONNECT);
            send_to_peer(msg, event.peer);
        } break;

        case ENET_EVENT_TYPE_DISCONNECT:
            // The disconnect handler registered in Router handles cleanup logic.
            break;

        case ENET_EVENT_TYPE_RECEIVE: {
            Message msg;
            msg.header.type =
                *reinterpret_cast<MessageType*>(event.packet->data);
            msg.header.size =
                event.packet->dataLength - sizeof(MessageType);

            msg.payload.assign(
                event.packet->data + sizeof(MessageType),
                event.packet->data + event.packet->dataLength
            );

            if (router_)
                router_->route(msg);

            enet_packet_destroy(event.packet);
        } break;

        default:
            break;
        }
    }
}

void Host::broadcast(const Message& message) noexcept
{
    if (clients_.empty())
        return;

    const MessageType type = message.header.type;
    const size_t packetSize =
        sizeof(MessageType) + message.payload.size();

    ENetPacket* packet = enet_packet_create(
        nullptr,
        packetSize,
        ENET_PACKET_FLAG_RELIABLE
    );

    if (!packet)
        return;

    std::memcpy(packet->data, &type, sizeof(MessageType));

    if (!message.payload.empty()) {
        std::memcpy(packet->data + sizeof(MessageType),
                    message.payload.data(),
                    message.payload.size());
    }

    // Send to all
    for (auto& [uuid, peer] : clients_) {
        enet_peer_send(peer, 0, packet);
    }
}

void Host::disconnect() noexcept
{
    if (!server_)
        return;

    connection_state_ = ConnectionState::DISCONNECTING;

    // Notify clients
    MsgDisconnect body{};
    std::strncpy(body.uuid, local_uuid_.c_str(), sizeof(body.uuid) - 1);

    Message msg = SerializeMessage(body, DefaultMessageTypes::HOST_DISCONNECT);
    broadcast(msg);

    enet_host_flush(server_);

    // Disconnect all clients
    for (auto& [uuid, peer] : clients_) {
        if (peer)
            enet_peer_disconnect_later(peer, 0);
    }

    clients_.clear();

    // Destroy server
    enet_host_destroy(server_);
    server_ = nullptr;

    connection_state_ = ConnectionState::DISCONNECTED;
}

void Host::send_to_peer(const Message& message, ENetPeer* peer) noexcept
{
    if (!peer)
        return;

    const MessageType type = message.header.type;
    const size_t packetSize =
        sizeof(MessageType) + message.payload.size();

    ENetPacket* packet = enet_packet_create(
        nullptr,
        packetSize,
        ENET_PACKET_FLAG_RELIABLE
    );

    if (!packet)
        return;

    std::memcpy(packet->data, &type, sizeof(MessageType));

    if (!message.payload.empty()) {
        std::memcpy(packet->data + sizeof(MessageType),
                    message.payload.data(),
                    message.payload.size());
    }

    enet_peer_send(peer, 0, packet);
}

ConnectionState Host::get_connection_state() const noexcept
{
    return connection_state_;
}

void Host::set_max_clients(int amount) noexcept
{
    max_clients_ = amount;
}

int Host::get_client_amount() const noexcept
{
    return static_cast<int>(clients_.size());
}

void Host::set_connection_port(int port) noexcept
{
    connection_port_ = port;
}

void Host::set_client_disconnect_handler() noexcept
{
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

    if (router_)
        router_->register_handler(DefaultMessageTypes::CLIENT_DISCONNECT,
                                  std::move(handler));
}
