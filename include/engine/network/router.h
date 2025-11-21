#pragma once

#include <unordered_map>
#include <functional>

#include <engine/network/network_message.h>

/**
 * @brief Routes incoming network messages to registered handlers.
 */
class Router {
public:
    /**
     * @brief Registers a handler for a specific message type.
     * If the handler already exists, it will be overwritten.
     */
    void register_handler(MessageType type, std::function<void(const Message&)> handler);

    /**
     * @brief Unregisters the handler associated with a message type.
     * @throws std::runtime_error if no handler exists.
     */
    void unregister_handler(MessageType type);

    /**
     * @brief Routes the message to its corresponding handler.
     * If no handler exists, the message is ignored.
     */
    void route(const Message& msg) const;

private:
    std::unordered_map<MessageType, std::function<void(const Message&)>> handlers_;
};
