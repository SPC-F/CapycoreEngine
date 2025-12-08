#include <engine/network/multiplayer_service.h>
#include <engine/network/snapshot.h>
#include <chrono>

#include <stdexcept>

MultiplayerService::MultiplayerService()
{
    if (enet_initialize() != 0) { // 0 on succes, < 0 on failure
        throw std::runtime_error("Failed to initialize ENet.");
    }

    atexit(enet_deinitialize);

    router_ = std::make_unique<Router>();
}

void MultiplayerService::register_handler(
    MessageType type,
    const std::function<void(const Message&)>& handler)
{
    router_->register_handler(type, handler);
}

void MultiplayerService::unregister_handler(MessageType type)
{
    router_->unregister_handler(type);
}

void MultiplayerService::set_host()
{
    // If currently client, ensure it isn't connected
    if (client_) {
        const auto state = client_->get_connection_state();
        if (state != ConnectionState::DISCONNECTED &&
            state != ConnectionState::NONE)
        {
            throw std::runtime_error("Cannot switch to host mode: client still connected.");
        }

        client_.reset();
    }

    host_ = std::make_unique<Host>(std::ref(*router_), connection_port_, max_clients_);
}

void MultiplayerService::set_client()
{
    // If currently host, ensure server isn't running
    if (host_) {
        const auto state = host_->get_connection_state();
        if (state != ConnectionState::DISCONNECTED &&
            state != ConnectionState::NONE)
        {
            throw std::runtime_error("Cannot switch to client mode: host server still active.");
        }

        host_.reset();
    }

    client_ = std::make_unique<Client>(std::ref(*router_));
}

PeerType MultiplayerService::get_peer_type() const
{
    if (host_) {
        return PeerType::HOST;
    } else if (client_) {
        return PeerType::CLIENT;
    } else {
        return PeerType::NONE;
    }
}

void MultiplayerService::poll()
{
    if (host_) {
        host_->poll();

        // Periodically create and broadcast delta snapshots
        if (snapshot_scene_) {
            const auto now = std::chrono::steady_clock::now();
            if (now - last_snapshot_time_ >= snapshot_interval_) {
                last_snapshot_time_ = now;
                Message delta = snapshot::create_delta_snapshot(*snapshot_scene_, DefaultMessageTypes::SNAPSHOT_DELTA);
                // Only broadcast if there is payload beyond the count header
                if (delta.header.size > sizeof(uint16_t)) {
                    host_->broadcast(delta);
                }
            }
        }
    }
    else if (client_) {
        client_->poll();
    }
}

void MultiplayerService::send(const Message& message)
{
    if (host_) {
        host_->broadcast(message);
    }
    else if (client_) {
        client_->send(message);
    }
    else {
        throw std::runtime_error("Cannot send: service is neither client nor host.");
    }
}

void MultiplayerService::send_to_uuid(const std::string& uuid, const Message& message)
{
    if (!host_) {
        throw std::runtime_error("send_to_uuid is only available in host mode.");
    }

    host_->send_to_uuid(uuid, message);
}

void MultiplayerService::start_server()
{
    if (client_) {
        throw std::runtime_error("Cannot start server while acting as client.");
    }

    if (!host_) {
        throw std::runtime_error("Host instance not initialized. Call set_host() first.");
    }

    host_->start_server();
}

void MultiplayerService::connect(const std::string& address)
{
    if (host_) {
        throw std::runtime_error("Cannot connect while acting as host.");
    }

    if (!client_) {
        throw std::runtime_error("Client instance not initialized. Call set_client() first.");
    }

    client_->connect(address, connection_port_);
}

void MultiplayerService::disconnect()
{
    if (host_) {
        host_->disconnect();
    }
    else if (client_) {
        client_->disconnect();
    }
    else {
        throw std::runtime_error("Cannot disconnect: service is neither client nor host.");
    }
}

ConnectionState MultiplayerService::get_connection_state() const noexcept
{
    if (host_)
        return host_->get_connection_state();
    if (client_)
        return client_->get_connection_state();
    return ConnectionState::NONE;
}

std::string MultiplayerService::get_uuid() const noexcept
{
    if (host_)
        return host_->get_uuid();
    if (client_)
        return client_->get_uuid();
    else
        throw std::runtime_error("Cannot get uuid: must be a host or connected client first.");
}

void MultiplayerService::set_max_clients(int amount) noexcept
{
    max_clients_ = amount;
    if (host_)
        host_->set_max_clients(amount);
}

int MultiplayerService::get_client_amount() const noexcept
{
    if (host_)
        return host_->get_client_amount();
    return 0;
}

void MultiplayerService::set_connection_port(int port) noexcept
{
    connection_port_ = port;
    if (host_)
        host_->set_connection_port(port);
}

int MultiplayerService::get_connection_port() const noexcept
{
    return connection_port_;
}

void MultiplayerService::enable_snapshots(class Scene& scene)
{
    snapshot_scene_ = &scene;

    // Register SNAPSHOT_FULL handler (both host and client use this)
    register_handler(MessageType(DefaultMessageTypes::SNAPSHOT_FULL),
        [this](const Message& msg) {
            if (snapshot_scene_) {
                snapshot::apply_full_snapshot(*snapshot_scene_, msg);
            }
        }
    );

    // Register SNAPSHOT_DELTA handler (client-side applies deltas)
    register_handler(MessageType(DefaultMessageTypes::SNAPSHOT_DELTA),
        [this](const Message& msg) {
            if (snapshot_scene_) {
                snapshot::apply_delta_snapshot(*snapshot_scene_, msg);
            }
        }
    );

    // Register handler to auto-send snapshots when a client connects (host only)
    // We intercept CONNECT to inject snapshot sending to the newly connected client
    auto on_client_connect = [this](const Message& msg) {
        // CONNECT message contains the client's UUID
        if (host_ && snapshot_scene_) {
            // Extract UUID from the CONNECT message
            struct MsgConnect { char uuid[37]; } data{};
            if (msg.payload.size() >= sizeof(data)) {
                std::memcpy(&data, msg.payload.data(), sizeof(data));
                
                Message snapshot_msg = snapshot::create_full_snapshot(*snapshot_scene_, DefaultMessageTypes::SNAPSHOT_FULL);
                host_->send_to_uuid(std::string(data.uuid), snapshot_msg);
            }
        }
    };

    // Hook to CONNECT event: when a client connects to the host, send them a snapshot
    register_handler(MessageType(DefaultMessageTypes::CONNECT),
        on_client_connect
    );
}

