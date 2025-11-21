#include <engine/network/host.h>
#include <engine/util/uuid.h>

#include <iostream>

Host::Host(std::shared_ptr<Router> router, int connection_port, int max_clients)
    : router_{router}, connection_port_{connection_port}, max_clients_{max_clients}, connection_state_{ConnectionState::NONE}, local_uuid_{uuid::generate_uuid_v4()}
{
    local_uuid_ = uuid::generate_uuid_v4();

    set_client_disconnect_handler();
}

Host::~Host()
{
    enet_host_flush(server_);
    enet_host_destroy(server_);
}

void Host::start_server()
{
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = connection_port_;

    server_ = enet_host_create (&address, max_clients_, 2, 0, 0);

    if (server_ == nullptr)
        throw "An error occurred while trying to create an ENet server host.";
        // TODO: This has never occurred during testing, how should this be handled?
}

void Host::poll() noexcept
{
    ENetEvent event;

    while (enet_host_service (server_, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                if (clients_.size() >= max_clients_)
                    enet_peer_disconnect(event.peer, 0);

                std::string uuid = uuid::generate_uuid_v4();
                clients_.emplace(uuid, event.peer);

                MsgConnect body;
                strcpy(body.uuid, uuid.c_str());
                Message message { SerializeMessage(body, DefaultMessageTypes::CONNECT) };

                send(message, event.peer);
            } break;

            case ENET_EVENT_TYPE_DISCONNECT:
                // Client disconnected.
            break;

            case ENET_EVENT_TYPE_RECEIVE:
                Message msg;
                msg.header.type = *reinterpret_cast<MessageType *>(event.packet->data);
                msg.header.size = event.packet->dataLength - sizeof(MessageType);
                msg.payload.assign(event.packet->data + sizeof(MessageType),
                                event.packet->data + event.packet->dataLength);

                router_->route(msg);

                enet_packet_destroy(event.packet);
            break;
        }
    }
}

void Host::broadcast(Message& message)
{
    MessageType type = message.header.type;
    size_t packetSize = sizeof(MessageType) + message.payload.size();
    ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);

    std::memcpy(packet->data, &type, sizeof(MessageType));
    std::memcpy(packet->data + sizeof(MessageType), message.payload.data(), message.payload.size());

    for (auto& it : clients_)
        enet_peer_send (it.second, 0, packet);
}

void Host::disconnect()
{
    connection_state_ = ConnectionState::DISCONNECTING;

    MsgDisconnect body;
    strcpy(body.uuid, local_uuid_.c_str());
    Message message { SerializeMessage(body, DefaultMessageTypes::HOST_DISCONNECT) };

    broadcast(message);
    enet_host_flush(server_);

    for (auto& it : clients_)
        enet_peer_disconnect_later(it.second, 0);
    clients_.clear();

    enet_host_destroy(server_);

    connection_state_ = ConnectionState::DISCONNECTED;
}

void Host::send(Message message, ENetPeer* peer)
{
    MessageType type = message.header.type;
    size_t packetSize = sizeof(MessageType) + message.payload.size();
    ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);

    std::memcpy(packet->data, &type, sizeof(MessageType));
    std::memcpy(packet->data + sizeof(MessageType), message.payload.data(), message.payload.size());

    enet_peer_send (peer, 0, packet);
}

const ConnectionState Host::get_connection_state() const noexcept
{
    return connection_state_;
}

void Host::set_max_clients(int amount) noexcept
{
    max_clients_ = amount;
}

const int Host::get_client_amount() const noexcept
{
    return clients_.size();
}

void Host::set_connection_port_(int port) noexcept
{
    connection_port_ = port;
}

void Host::set_client_disconnect_handler() noexcept
{
    auto HostOnPeerDisconnectHandler = [this](const Message& message) {
        MsgConnect data;
        std::memcpy(&data, message.payload.data(), sizeof(data));

        if(clients_[data.uuid]) {
            enet_peer_disconnect(clients_[data.uuid], 0);
            clients_.erase(data.uuid);
            // TODO: Broadcast disconnection
        }
    };
    router_->register_handler(DefaultMessageTypes::CLIENT_DISCONNECT, HostOnPeerDisconnectHandler);
}
