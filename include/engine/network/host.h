#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <enet/enet.h>

#include <engine/network/router.h>
#include <engine/network/connection_state.h>

class Host {
public:
    Host(std::shared_ptr<Router> router, int connection_port, int max_clients);
    ~Host();

    void start_server();

    /* @brief Polling network for new messages. */
    void poll() noexcept;

    /* @brief Broadcasting message to all clients. */
    void broadcast(Message& message);

    /* @brief Disconnect all clients. */
    void disconnect();

    [[nodiscard]] const ConnectionState get_connection_state() const noexcept;

    void set_max_clients(const int amount) noexcept;
    [[nodiscard]] const int get_client_amount() const noexcept;

    void set_connection_port_(const int port) noexcept;

private:
    ENetHost* server_;
    int connection_port_;
    int max_clients_;
    ConnectionState connection_state_;

    std::string local_uuid_;
    std::shared_ptr<Router> router_;
    std::unordered_map<std::string, ENetPeer*> clients_;

    /* @brief Send message to specific user. Used only to send uuid to register user. */
    void send(Message message, ENetPeer* peer);

    /* @brief Called on client disconnection. Cleaning data of client and forwarding it to custom handler. */
    void set_client_disconnect_handler() noexcept;
};
