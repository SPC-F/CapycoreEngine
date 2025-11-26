#include <engine/core/system/system_service.h>
#include <engine/core/system/strategy/sdl_backend_system.h>

SystemService::SystemService()
    : backend_system_(std::make_unique<SDLBackendSystem>())
{
    backend_system_->initialize_listeners(&listeners_);
}

void SystemService::add_listener(SystemEvent event, EventCallback callback)
{
    listeners_[event].push_back(std::move(callback));
}

void SystemService::update()
{
    backend_system_->poll_events();
}

void SystemService::init_frame_timer()
{
    backend_system_->init_frame_timer();
}

void SystemService::update_frame_time(float time_scale)
{
    backend_system_->update_frame_time(time_scale);
}

float SystemService::delta_time() const
{
    return backend_system_->delta_time();
}