#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <enet/enet.h>

#include <engine/network/router.h>
#include <engine/network/connection_state.h>

/**
 * @class Host
 * @brief High-level ENet-based server host for handling multiple client
 *        connections, broadcasting messages, and routing incoming packets.
 *
 * The Host object owns its ENetHost and manages the lifecycle of all connected
 * clients. It interacts with the Router to forward incoming messages to
 * application-defined handlers.
 */
class Host {
public:
    /**
     * @brief Constructs the Host with a Router, target port, and maximum number of clients.
     *
     * @param router Shared pointer to message routing system.
     * @param connection_port Port on which to listen for incoming clients.
     * @param max_clients Maximum number of simultaneous connections allowed.
     */
    Host(std::shared_ptr<Router> router, int connection_port, int max_clients);

    /**
     * @brief Destructs the Host and releases ENet resources safely.
     */
    ~Host() noexcept;

    /**
     * @brief Starts the ENet server.
     *
     * @throws std::runtime_error if ENet fails to create a server host.
     */
    void start_server();

    /**
     * @brief Polls ENet for incoming events (connect/disconnect/receive).
     *        Forwards packets through the Router.
     */
    void poll() noexcept;

    /**
     * @brief Broadcasts a message to all connected peers.
     * @param message Message to broadcast.
     */
    void broadcast(const Message& message) noexcept;

    /**
     * @brief Disconnects all clients and destroys the ENet server.
     */
    void disconnect() noexcept;

    /**
     * @brief Returns the current server connection state.
     */
    [[nodiscard]] ConnectionState get_connection_state() const noexcept;

    /**
     * @brief Sets the maximum number of clients allowed.
     */
    void set_max_clients(int amount) noexcept;

    /**
     * @brief Returns the number of currently connected clients.
     */
    [[nodiscard]] int get_client_amount() const noexcept;

    /**
     * @brief Sets the server port. Only effective before start_server().
     */
    void set_connection_port(int port) noexcept;

private:
    ENetHost* server_{nullptr};
    int connection_port_{0};
    int max_clients_{0};
    ConnectionState connection_state_{ConnectionState::NONE};

    std::string local_uuid_;
    std::shared_ptr<Router> router_{nullptr};

    // Maps client UUID to the peer object assigned by ENet.
    std::unordered_map<std::string, ENetPeer*> clients_;

    /**
     * @brief Sends a message to a specific peer (internal use only).
     * @param message Message to send.
     * @param peer Target peer.
     */
    void send_to_peer(const Message& message, ENetPeer* peer) noexcept;

    /**
     * @brief Registers internal disconnect handler to clean client state
     *        and forward disconnection events.
     */
    void set_client_disconnect_handler() noexcept;
};
