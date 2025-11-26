#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include <engine/core/events/system_event.h>

using BackendCallback = std::function<void(void* event_data)>;

class IBackendSystem
{
public:
    virtual ~IBackendSystem() = default;

    virtual void initialize_listeners(
        std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners
    ) = 0;

    virtual void poll_events() = 0;
};
