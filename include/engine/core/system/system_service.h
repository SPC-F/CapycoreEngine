#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "engine/core/events/system_event.h"
#include "engine/core/iEngineService.h"
#include "engine/core/system/i_backend_system.h"


class SystemService : public IEngineService
{
public:
    explicit SystemService();

    using EventCallback = std::function<void(void* event)>;

    void add_listener(SystemEvent event, EventCallback callback);
    void update();

    void init_frame_timer();
    void update_frame_time(float time_scale);
    [[nodiscard]] float delta_time() const;

private:
    std::unique_ptr<IBackendSystem> backend_system_;
    std::unordered_map<SystemEvent, std::vector<EventCallback>> listeners_;
};