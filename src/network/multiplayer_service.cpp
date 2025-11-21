#include <engine/network/multiplayer_service.h>

#include <iostream>

MultiplayerService::MultiplayerService()
{
    if (enet_initialize() != 0)
        throw "An error occurred while initializing ENet.";

    atexit (enet_deinitialize);

    router_ = std::make_shared<Router>();
}

void MultiplayerService::register_handler(const MessageType type, const std::function<void(const Message&)> handler)
{
    router_->register_handler(type, handler);
}

void MultiplayerService::unregister_handler(const MessageType type)
{
    router_->unregister_handler(type);
}

void MultiplayerService::set_host()
{
    if (client_)
        throw "Cannot act as host; Already acting as client.";

    host_ = std::make_unique<Host>(router_, connection_port_);
}

void MultiplayerService::set_client()
{
    if (host_)
        throw "Cannot act as client; Already acting as host.";

    client_ = std::make_unique<Client>(router_);
}

void MultiplayerService::poll()
{
    if (host_)
        host_->poll();
    else if (client_)
        client_->poll();
    else
        throw "Cannot poll. Not acting as host or client.";
}

void MultiplayerService::send(Message& message)
{

}

void MultiplayerService::start_server()
{
    if (client_)
        throw "Cannot start server as client.";

    host_->start_server();
}

void MultiplayerService::connect(const std::string& address)
{
    if (host_)
        throw "Cannot initiate connection as host.";

    client_->connect(address, connection_port_);
}

void MultiplayerService::disconnect()
{
    if (host_)
        host_->disconnect();
    else if (client_)
        client_->disconnect();
    else
        throw "Cannot disconnect. Not acting as host or client.";
}

ConnectionState MultiplayerService::get_connection_state() noexcept
{
    if (host_)
        return host_->get_connection_state();
    else if (client_)
        return client_->get_connection_state();
    else
        return ConnectionState::NONE;
}

void MultiplayerService::set_max_clients(int amount)
{
    if (host_)
        host_->set_max_clients(amount);
}

int MultiplayerService::get_client_amount() noexcept
{
    if (host_)
        return host_->get_client_amount();

    return 0;
}

void MultiplayerService::set_connection_port(int port)
{
    connection_port_ = port;

    if (host_)
        host_->set_connection_port_(port);
}

int MultiplayerService::get_connection_port() noexcept
{
    return connection_port_;
}
