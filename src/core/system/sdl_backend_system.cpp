#include <engine/core/system/sdl_backend_system.h>

SDLBackendSystem::SDLBackendSystem()
{}

void SDLBackendSystem::initialize_listeners(
    std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners
)
{
    listeners_ = listeners;
}

void SDLBackendSystem::poll_events()
{
    SDL_Event e;

    auto execute_callbacks = [this](SystemEvent event_type, SDL_Event& event) {
        for (auto& cb : (*listeners_)[event_type]) {
            cb(&event);
        }
    };

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:                execute_callbacks(EVENT_QUIT, e); break;
            case SDL_EVENT_KEY_DOWN:            execute_callbacks(KEY_DOWN, e); break;
            case SDL_EVENT_KEY_UP:              execute_callbacks(KEY_UP, e); break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:   execute_callbacks(MOUSE_BUTTON_DOWN, e); break;
            case SDL_EVENT_MOUSE_BUTTON_UP:     execute_callbacks(MOUSE_BUTTON_UP, e); break;
            case SDL_EVENT_MOUSE_MOTION:        execute_callbacks(MOUSE_MOTION, e); break;
            case SDL_EVENT_MOUSE_WHEEL:         execute_callbacks(MOUSE_WHEEL, e); break;

            // Handle other SDL events as needed
            default: break;
        }
    }
}