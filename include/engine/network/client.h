#pragma once

#include <memory>
#include <string>
#include <enet/enet.h>

#include <engine/network/router.h>
#include <engine/network/connection_state.h>

/**
 * @class Client
 * @brief High-level ENet-based network client. Handles connecting,
 *        disconnecting, sending messages, and routing received messages.
 *
 * The Client owns its ENetHost and manages lifecycle events. Incoming
 * messages are forwarded through the Router for user-defined handling.
 */
class Client {
public:
    /**
     * @brief Construct a Client with a shared Router.
     * @param router Pointer to router used for dispatching messages.
     *
     * @throws std::runtime_error if ENet client host creation fails.
     */
    explicit Client(std::shared_ptr<Router> router);

    /**
     * @brief Safely destroys the client, unregisters handlers and frees ENet resources.
     */
    ~Client() noexcept;

    /**
     * @brief Polls the network for incoming ENet events.
     *        Routes received messages and updates connection state.
     */
    void poll() noexcept;

    /**
     * @brief Sends a message to the connected server peer.
     * @param message Message to send.
     */
    void send(const Message& message) noexcept;

    /**
     * @brief Initiates connection to a remote host.
     * @param host_ip Target IPv4/IPv6 address.
     * @param connection_port Target port.
     *
     * @throws std::runtime_error if ENet cannot allocate a peer.
     */
    void connect(const std::string& host_ip, int connection_port);

    /**
     * @brief Begins a clean disconnect by sending a disconnect message.
     */
    void disconnect() noexcept;

    /**
     * @brief Returns current connection state.
     */
    [[nodiscard]] ConnectionState get_connection_state() const noexcept;

private:
    std::string local_uuid_;
    std::shared_ptr<Router> router_{nullptr};
    ENetPeer* server_peer_{nullptr};
    ENetHost* client_{nullptr};
    ConnectionState connection_state_{ConnectionState::NONE};

    /**
     * @brief Registers internal callback for handling connect messages.
     */
    void register_on_connect_handler() noexcept;

    /**
     * @brief Registers internal callback for handling disconnect messages.
     */
    void register_on_disconnect_handler() noexcept;
};
