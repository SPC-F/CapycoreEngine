#pragma once

#include <memory>
#include <enet/enet.h>

#include <engine/network/router.h>

class Client {
public:
    Client(std::shared_ptr<Router> router);

    /* @brief polling network for new messages. */
    void poll() noexcept;

    /* @brief sending message to host. */
    void send(Message& message);

    /* @brief Connect to given address. */
    void connect(std::string address);

    /* @brief Disconnect from current peer. */
    void disconnect();

    /* @brief Called on connection, setting local uuid and forwarding to custom handlers. */
    void on_connect() noexcept;

    /* @brief Called on disconnection, cleaning left-over data and forwarding to custom handlers. */
    void on_disconnect() noexcept;

private:
    std::shared_ptr<Router> router_{nullptr};
    std::unique_ptr<ENetPeer> server_peer_{nullptr};
    std::unique_ptr<ENetHost> client_{nullptr};
};
