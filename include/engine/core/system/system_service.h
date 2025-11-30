#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include "engine/core/events/system_event.h"
#include "engine/core/iEngineService.h"
#include "engine/core/system/i_backend_system.h"


class SystemService : public IEngineService
{
public:
    explicit SystemService();

    using EventCallback = std::function<void(void* event)>;

    int add_listener(SystemEvent event, EventCallback callback); // NOLINT
    void remove_listener(SystemEvent event, int listener_id); // NOLINT
    
    void update();
    
    std::string get_clipboard_text();
    void set_cursor_to_arrow();
    void set_cursor_to_hand();
    void set_cursor_to_ibeam();
    void set_cursor_to_crosshair();
    void set_cursor_to_wait();

    void init_frame_timer();
    void update_frame_time(float time_scale);
    [[nodiscard]] float delta_time() const;

private:
    std::unique_ptr<IBackendSystem> backend_system_;
    std::unordered_map<SystemEvent, std::vector<EventCallback>> listeners_;
};