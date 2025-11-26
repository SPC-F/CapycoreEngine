#pragma once

#include <SDL3/SDL.h>

#include <engine/core/system/i_backend_system.h>

class SDLBackendSystem : public IBackendSystem
{
public:
    SDLBackendSystem();
    ~SDLBackendSystem() override;

    void initialize_listeners(
        std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners
    ) override;

    void poll_events() override;
private:
    std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners_{ nullptr };
};