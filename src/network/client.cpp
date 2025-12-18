#include <engine/network/client.h>
#include <engine/network/host.h>
#include <engine/network/snapshot.h>
#include <engine/core/engine.h>
#include <engine/public/scene_service.h>

#include <cstring>
#include <stdexcept>

Client::Client(std::reference_wrapper<Router> router)
    : router_(router)
{
    const ENetAddress* address = nullptr;
    size_t peer_count = 1;
    size_t channel_limit = 2;
    enet_uint32 max_bandwidth = 0;

    client_ = enet_host_create(
        address,
        peer_count,
        channel_limit,
        max_bandwidth,
        max_bandwidth
    );

    if (client_ == nullptr)
        throw std::runtime_error("Failed to create ENet client host.");

    register_on_connect_handler();
    register_on_disconnect_handler();
    register_on_snapshot_full_handler();
    register_on_snapshot_delta_handler();
}

Client::~Client() noexcept
{
    if (server_peer_) {
        enet_peer_reset(server_peer_);
        server_peer_ = nullptr;
    }

    if (client_) {
        enet_host_flush(client_);
        enet_host_destroy(client_);
        client_ = nullptr;
    }

    router_.get().unregister_handler(DefaultMessageTypes::HOST_DISCONNECT);
    router_.get().unregister_handler(DefaultMessageTypes::CONNECT);
    router_.get().unregister_handler(DefaultMessageTypes::SNAPSHOT_FULL);
    router_.get().unregister_handler(DefaultMessageTypes::SNAPSHOT_DELTA);
}

void Client::poll() noexcept
{
    if (!client_)
        return;

    ENetEvent event{};

    while (enet_host_service(client_, &event, 0) > 0) {
        switch (event.type) {

        case ENET_EVENT_TYPE_CONNECT:
            // Waiting for server UUID message.
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            if (connection_state_ == ConnectionState::DISCONNECTING) {
                event.peer->data = nullptr;
                enet_peer_reset(event.peer);
                server_peer_ = nullptr;
                connection_state_ = ConnectionState::DISCONNECTED;
            } else {
                // Unexpected disconnect. Could attempt reconnection logic here.
                connection_state_ = ConnectionState::DISCONNECTED;
            }
            break;

        case ENET_EVENT_TYPE_RECEIVE: {
            Message msg;

            uint16_t raw_type;
            std::memcpy(&raw_type, event.packet->data, sizeof(raw_type));
            msg.header.type = MessageType::from_raw(raw_type);

            msg.header.size =
                event.packet->dataLength - sizeof(raw_type);

            msg.payload.assign(
                event.packet->data + sizeof(raw_type),
                event.packet->data + event.packet->dataLength
            );

            router_.get().route(msg);

            enet_packet_destroy(event.packet);
            break;
        }

        default:
            break;
        }
    }
}

void Client::send(const Message& message) noexcept
{
    if (!server_peer_)
        return;

    uint16_t raw_type = MessageType::to_raw(message.header.type);
    const size_t packet_size = sizeof(raw_type) + message.payload.size();

    ENetPacket* packet = enet_packet_create(
        nullptr,
        packet_size,
        ENET_PACKET_FLAG_RELIABLE
    );

    if (!packet)
        return;

    std::memcpy(packet->data, &raw_type, sizeof(raw_type));

    if (!message.payload.empty()) {
        std::memcpy(packet->data + sizeof(raw_type),
                    message.payload.data(),
                    message.payload.size());
    }

    enet_peer_send(server_peer_, 0, packet);
}

void Client::connect(const std::string& host_ip, int connection_port)
{
    connection_state_ = ConnectionState::CONNECTING;

    ENetAddress address{};
    enet_address_set_host(&address, host_ip.c_str());
    address.port = connection_port;

    size_t channel_limit = 2;
    enet_uint32 data = 0;

    server_peer_ = enet_host_connect(
        client_,
        &address,
        channel_limit,
        data
    );

    if (server_peer_ == nullptr)
        throw std::runtime_error("ENet could not create a peer for connection attempt.");

    // A timed connection attempt could be implemented here.
}

void Client::disconnect() noexcept
{
    if (connection_state_ == ConnectionState::DISCONNECTING ||
        connection_state_ == ConnectionState::DISCONNECTED)
        return;

    connection_state_ = ConnectionState::DISCONNECTING;

    MsgDisconnect body{};
    std::strncpy(body.uuid, local_uuid_.c_str(), sizeof(body.uuid) - 1);

    Message message{ serialize_message(body, DefaultMessageTypes::CLIENT_DISCONNECT) };
    send(message);
}

ConnectionState Client::get_connection_state() const noexcept
{
    return connection_state_;
}

std::string Client::get_uuid() const noexcept
{
    return local_uuid_;
}

void Client::register_on_connect_handler() noexcept
{
    auto handler = [this](const Message& message) {
        MsgConnect data{};
        std::memcpy(&data, message.payload.data(), sizeof(data));

        local_uuid_ = data.uuid;
        connection_state_ = ConnectionState::CONNECTED;
    };

    router_.get().register_handler(DefaultMessageTypes::CONNECT, std::move(handler));
}

void Client::register_on_disconnect_handler() noexcept
{
    auto handler = [this](const Message&) {
        connection_state_ = ConnectionState::DISCONNECTING;
    };

    router_.get().register_handler(DefaultMessageTypes::HOST_DISCONNECT, std::move(handler));
}

void Client::register_on_snapshot_full_handler() noexcept
{
    auto handler = [this](const Message& msg) {
        auto& engine = Engine::instance();
        auto& scene_service = engine.services->get_service<SceneService>().get();

        snapshot::apply_full_snapshot(scene_service.current_scene().value(), msg);
    };

    router_.get().register_handler(MessageType(DefaultMessageTypes::SNAPSHOT_FULL), std::move(handler));
}

void Client::register_on_snapshot_delta_handler() noexcept
{
    auto handler = [this](const Message& msg) {
        auto& engine = Engine::instance();
        auto& scene_service = engine.services->get_service<SceneService>().get();

        snapshot::apply_delta_snapshot(scene_service.current_scene().value(), msg);
    };

    router_.get().register_handler(MessageType(DefaultMessageTypes::SNAPSHOT_DELTA), std::move(handler));
}
