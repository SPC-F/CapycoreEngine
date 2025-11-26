#include <engine/core/system/system_service.h>
#include <engine/core/system/sdl_backend_system.h>

SystemService::SystemService()
    : backend_system_(std::make_unique<SDLBackendSystem>())
{
    backend_system_->initialize_listeners(&listeners_);
}

void SystemService::add_listener(SystemEvent event, EventCallback callback)
{
    listeners_[event].push_back(callback);
}

void SystemService::update()
{
    backend_system_->poll_events();
}
