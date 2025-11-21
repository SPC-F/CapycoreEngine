#include <engine/network/router.h>
#include <stdexcept>

void Router::register_handler(MessageType type, std::function<void(const Message&)> handler)
{
    handlers_[type] = std::move(handler);
}

void Router::unregister_handler(MessageType type)
{
    auto it = handlers_.find(type);
    if (it == handlers_.end()) {
        throw std::runtime_error("Cannot unregister handler: no handler registered for given MessageType.");
    }

    handlers_.erase(it);
}

void Router::route(const Message& msg) const
{
    auto it = handlers_.find(msg.header.type);
    if (it != handlers_.end()) {
        it->second(msg);
    }
    // else: unknown message type - intentionally ignored
}
