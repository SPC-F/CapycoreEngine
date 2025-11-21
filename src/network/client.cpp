#include <engine/network/client.h>
#include <engine/network/host.h>

#include <cstring>
#include <stdexcept>

Client::Client(std::shared_ptr<Router> router)
    : router_(std::move(router))
{
    client_ = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client_ == nullptr)
        throw std::runtime_error("Failed to create ENet client host.");

    register_on_connect_handler();
    register_on_disconnect_handler();
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

    if (router_) {
        router_->unregister_handler(DefaultMessageTypes::HOST_DISCONNECT);
        router_->unregister_handler(DefaultMessageTypes::CONNECT);
    }
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
            msg.header.type = *reinterpret_cast<MessageType*>(event.packet->data);
            msg.header.size = event.packet->dataLength - sizeof(MessageType);

            msg.payload.assign(
                event.packet->data + sizeof(MessageType),
                event.packet->data + event.packet->dataLength
            );

            if (router_)
                router_->route(msg);

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

    const MessageType type = message.header.type;
    const size_t packetSize = sizeof(MessageType) + message.payload.size();

    ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);
    if (!packet)
        return; // Best-effort send; no exception in noexcept function.

    std::memcpy(packet->data, &type, sizeof(MessageType));
    if (!message.payload.empty()) {
        std::memcpy(packet->data + sizeof(MessageType),
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

    server_peer_ = enet_host_connect(client_, &address, 2, 0);

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

    Message message{ SerializeMessage(body, DefaultMessageTypes::CLIENT_DISCONNECT) };
    send(message);
}

ConnectionState Client::get_connection_state() const noexcept
{
    return connection_state_;
}

void Client::register_on_connect_handler() noexcept
{
    auto handler = [this](const Message& message) {
        MsgConnect data{};
        std::memcpy(&data, message.payload.data(), sizeof(data));

        local_uuid_ = data.uuid;
        connection_state_ = ConnectionState::CONNECTED;
    };

    if (router_)
        router_->register_handler(DefaultMessageTypes::CONNECT, std::move(handler));
}

void Client::register_on_disconnect_handler() noexcept
{
    auto handler = [this](const Message&) {
        connection_state_ = ConnectionState::DISCONNECTING;
    };

    if (router_)
        router_->register_handler(DefaultMessageTypes::HOST_DISCONNECT, std::move(handler));
}
