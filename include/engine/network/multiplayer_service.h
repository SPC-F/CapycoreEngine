#pragma once

#include <memory>
#include <functional>
#include <string>
#include <enet/enet.h>

#include <engine/core/iEngineService.h>
#include <engine/network/client.h>
#include <engine/network/host.h>
#include <engine/network/router.h>
#include <engine/network/connection_state.h>

/**
 * @class MultiplayerService
 * @brief High-level peer-to-peer networking service managing host/client mode,
 *        message routing, and network lifecycle operations.
 *
 * The MultiplayerService provides a unified API for switching between
 * client-mode and host-mode, forwarding messages, and polling network events.
 * It wraps ENet initialization and manages both Host and Client instances.
 */
class MultiplayerService : public IEngineService {
public:
    /**
     * @brief Initializes ENet and prepares the internal Router.
     * @throws std::runtime_error if ENet initialization fails.
     */
    MultiplayerService();

    /**
     * @brief Registers a callback handler for a specific message type.
     * @param type Message type
     * @param handler Function invoked on message reception
     */
    void register_handler(MessageType type,
                          const std::function<void(const Message&)>& handler);

    /**
     * @brief Removes a previously registered handler for a message type.
     */
    void unregister_handler(MessageType type);

    /**
     * @brief Switches the service into host-mode. Invalidates any client instance.
     * @throws std::runtime_error if the current client has an active connection.
     */
    void set_host();

    /**
     * @brief Switches the service into client-mode. Invalidates any host instance.
     * @throws std::runtime_error if the current host has active clients or running server.
     */
    void set_client();

    /**
     * @brief Polls the network. Behavior depends on host/client mode.
     */
    void poll();

    /**
     * @brief Sends a message. Host broadcasts; client sends to server.
     */
    void send(const Message& message);

    /**
     * @brief Starts the host server. Only valid in host-mode.
     * @throws std::runtime_error if called while acting as client.
     */
    void start_server();

    /**
     * @brief Connect to a remote server. Only valid in client-mode.
     */
    void connect(const std::string& address);

    /**
     * @brief Disconnect the active host or client gracefully.
     * @throws std::runtime_error if not acting as host or client.
     */
    void disconnect();

    /**
     * @brief Returns connection state of the current mode.
     */
    [[nodiscard]] ConnectionState get_connection_state() const noexcept;

    void set_max_clients(int amount) noexcept;
    [[nodiscard]] int get_client_amount() const noexcept;

    void set_connection_port(int port) noexcept;
    [[nodiscard]] int get_connection_port() const noexcept;

private:
    std::shared_ptr<Router> router_{nullptr};
    std::unique_ptr<Client> client_{nullptr};
    std::unique_ptr<Host> host_{nullptr};

    int connection_port_{1024};
    int max_clients_{4};
};
