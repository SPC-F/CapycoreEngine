#pragma once

#include <unordered_map>
#include <any>
#include <string>
#include <functional>
#include <enet/enet.h>

#include <engine/network/network_message.h>

/**
 * @brief Router used for sending incoming network messages to corresponding handlers.
 */
class Router {
public:
    /* @brief Register new handler via given message type. */
    void register_handler(const MessageType type, const std::function<void(const Message&)> handler);

    /* @brief Unregister handler via given message type. */
    void unregister_handler(const MessageType type);

    /* @brief Route date to handler of given type. */
    void route(const Message& msg);

private:
    std::unordered_map<MessageType, std::function<void(const Message&)>> handlers_;
};
