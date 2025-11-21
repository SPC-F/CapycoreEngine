#include <engine/network/client.h>
#include <engine/network/host.h>
#include <iostream>

Client::Client(std::shared_ptr<Router> router)
{
    router_ = router;
    client_ = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client_ == nullptr)
        throw "An error occurred while trying to create an ENet client host.";
        // TODO: This has never occurred during testing, how should this be handled?

    auto ClientOnPeerConnectHandler = [this](const Message& message) {
        MsgConnect data;
        std::memcpy(&data, message.payload.data(), sizeof(data));

        local_uuid_ = data.uuid;
        std::cout << "New UUID: " << local_uuid_ << std::endl;

        connection_state_ = ConnectionState::CONNECTED;
    };
    router_->register_handler(DefaultMessageTypes::CONNECT, ClientOnPeerConnectHandler);

    auto ClientOnPeerDisconnectHandler = [this](const Message& message) {
        connection_state_ = ConnectionState::DISCONNECTING;
        std::cout << "Disconnecting!" << std::endl;
    };
    router_->register_handler(DefaultMessageTypes::HOST_DISCONNECT, ClientOnPeerDisconnectHandler);
}

Client::~Client()
{
    if (server_peer_)
        enet_peer_reset(server_peer_);

    enet_host_flush(client_);
    enet_host_destroy(client_);

    router_->unregister_handler(DefaultMessageTypes::HOST_DISCONNECT);
    router_->unregister_handler(DefaultMessageTypes::CONNECT);
}

void Client::poll() noexcept
{
    ENetEvent event;

    while (enet_host_service(client_, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connected!" << std::endl;
                // Connection succesfully established.
                // I am unsure what should be used here.
                // We are connected, but haven't received our uuid yet.
            break;

            case ENET_EVENT_TYPE_DISCONNECT:
                if (connection_state_ == ConnectionState::DISCONNECTING) {
                    event.peer -> data = NULL;
                    connection_state_ = ConnectionState::DISCONNECTED;
                    enet_peer_reset(event.peer);
                    server_peer_ = nullptr;
                    std::cout << "Disconnected!" << std::endl;
                    // TODO: Clean everything
                } else {
                    // TODO: Something went wrong, try reconnecting
                    std::cout << "Uhhh..." << std::endl;
                }
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

void Client::send(Message& message)
{
    MessageType type = message.header.type;
    size_t packetSize = sizeof(MessageType) + message.payload.size();
    ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);

    std::memcpy(packet->data, &type, sizeof(MessageType));
    std::memcpy(packet->data + sizeof(MessageType), message.payload.data(), message.payload.size());

    enet_peer_send (server_peer_, 0, packet);
}

void Client::connect(const std::string& host_ip, const int connection_port)
{
    connection_state_ = ConnectionState::CONNECTING;

    ENetAddress address;
    ENetEvent event;
    std::cout << "Connecting to: " << host_ip << ":" << connection_port << std::endl;
    enet_address_set_host (&address, host_ip.c_str());
    address.port = connection_port;

    server_peer_ = enet_host_connect(client_, &address, 2, 0);
    if (server_peer_ == nullptr)
        throw "No available peers for initiating an ENet connection";
        // TODO: This has never occurred during testing, how should this be handled?

    // TODO: If not connecting within a given time, forcefully disconnect
}

void Client::disconnect()
{
    connection_state_ = ConnectionState::DISCONNECTING;

    MsgDisconnect body;
    strcpy(body.uuid, local_uuid_.c_str());
    Message message { SerializeMessage(body, DefaultMessageTypes::CLIENT_DISCONNECT) };

    send(message);
}

void Client::on_connect() noexcept
{

}

void Client::on_disconnect() noexcept
{

}

ConnectionState Client::get_connection_state() noexcept
{
    return connection_state_;
}
