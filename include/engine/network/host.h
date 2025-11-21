#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <enet/enet.h>

#include <engine/network/router.h>
#include <engine/network/connection_state.h>

class Host {
public:
    Host(std::shared_ptr<Router> router, int connection_port);
    ~Host();

    void start_server();

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

    ConnectionState get_connection_state() noexcept;

    void set_max_clients(int amount);
    int get_client_amount() noexcept;

    void set_connection_port_(int port);

private:
    ENetHost* server_;
    int connection_port_;

    int max_clients_{4};
    std::string local_uuid_;
    std::shared_ptr<Router> router_{nullptr};
    std::unordered_map<std::string, ENetPeer*> clients_;

    ConnectionState connection_state_{ConnectionState::NONE};

    /* @brief Send message to specific user. Used only to send uuid to register user. */
    void send(Message message, ENetPeer* peer);
};
