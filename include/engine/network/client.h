#pragma once

#include <memory>
#include <string>
#include <enet/enet.h>

#include <engine/network/router.h>
#include <engine/network/connection_state.h>

class Client {
public:
    Client(std::shared_ptr<Router> router);
    ~Client();

    /* @brief polling network for new messages. */
    void poll() noexcept;

    /* @brief sending message to host. */
    void send(Message& message);

    /* @brief Connect to given address. */
    void connect(const std::string& host_ip, const int connection_port);

    /* @brief Disconnect from current peer. */
    void disconnect();

    [[nodiscard]] ConnectionState get_connection_state() noexcept;

private:
    std::string local_uuid_;
    std::shared_ptr<Router> router_{nullptr};
    ENetPeer* server_peer_{nullptr};
    ENetHost* client_{nullptr};
    ConnectionState connection_state_{ConnectionState::NONE};

    /* @brief Called on connection, setting local uuid and forwarding to custom handlers. */
    void register_on_connect_handler() noexcept;

    /* @brief Called on disconnection, cleaning left-over data and forwarding to custom handlers. */
    void register_on_disconnect_handler() noexcept;
};
