#include <engine/network/router.h>

void Router::register_handler(const MessageType type, const std::function<void(const Message&)> handler)
{
    handlers_.emplace(type, handler);
}

void Router::unregister_handler(const MessageType type)
{
    if (!handlers_[type])
        throw "No handler of given message type found.";

    handlers_.erase(type);
}

void Router::route(const Message& msg)
{
    if (auto it = handlers_[msg.header.type])
        it(msg);
    else {
        // Handle unknown message type if necessary
    }
}
