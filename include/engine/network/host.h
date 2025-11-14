#pragma once

#include <memory>
#include <unordered_map>
#include <enet/enet.h>

#include <engine/network/router.h>

class Host {
public:
    Host(std::shared_ptr<Router> router);

    /* @brief Polling network for new messages. */
    void poll() noexcept;

    /* @brief Broadcasting message to all clients. */
    void broadcast(Message& message);

    /* @brief Disconnect all clients. */
    void disconnect();

    /* @brief Called on new connection with client. Registering new client with new uuid, forwarding it to custom handler and sending uuid to new client. */
    void on_client_connect() noexcept;

    /* @brief Called on client disconnection. Cleaning data of client and forwarding it to custom handler. */
    void on_client_disconnect() noexcept;

private:
    int max_clients_{4};
    std::shared_ptr<Router> router_{nullptr};
    std::unordered_map<int, std::unique_ptr<ENetHost>> clients_;

    /* @brief Send message to specific user. Used only to send uuid to register user. */
    void send(std::string uuid, Message& message);
};
