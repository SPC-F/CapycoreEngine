#include <engine/network/multiplayer_service.h>

#include <stdexcept>

MultiplayerService::MultiplayerService()
{
    if (enet_initialize() != 0) {
        throw std::runtime_error("Failed to initialize ENet.");
    }

    atexit(enet_deinitialize);

    router_ = std::make_shared<Router>();
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

    host_ = std::make_unique<Host>(router_, connection_port_, max_clients_);
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

    client_ = std::make_unique<Client>(router_);
}

void MultiplayerService::poll()
{
    if (host_) {
        host_->poll();
    }
    else if (client_) {
        client_->poll();
    }
    else {
        throw std::runtime_error("Cannot poll: service is neither client nor host.");
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
