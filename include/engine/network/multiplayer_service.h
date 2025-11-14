#pragma once

#include <memory>

#include <engine/core/iEngineService.h>
#include <engine/network/client.h>
#include <engine/network/host.h>
#include <engine/network/router.h>

/**
 * @brief Service containing all functions to properly manage a peer-to-peer connection.
 *
 * The service contains delegator methods for all useful functionalities.
 */
class MultiplayerService : public IEngineService {
public:
    /* @brief Register handler function to message type. */
    void register_handler(const MessageType type, const std::function<void()>& handler);

    /* @brief Unregister handler of given message type */
    void unregister_handler(const MessageType type);

    /* @brief Register self as host (mutually exclusive with client). */
    void set_host();

    /* @brief Register self as client (mutually exclusive with host). */
    void set_client();

    /* @brief Poll network for new messages. */
    void poll();

    /* @brief If client, send to host; If host, broadcast to all clients. */
    void send(Message& message);

    /* @brief Connect to given address. Only usable as client. */
    void connect(const std::string& address);

    /* @brief If client, disconnect from host; If host, disconnect all clients. */
    void disconnect();

private:
    std::shared_ptr<Router> router_;
    std::unique_ptr<Client> client_;
    std::unique_ptr<Host> host_;
};
