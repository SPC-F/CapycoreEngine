#include <engine/core/system/strategy/sdl_backend_system.h>

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

            default: break;
        }
    }
}

void SDLBackendSystem::init_frame_timer() {
    last_ = SDL_GetPerformanceCounter();
    freq_ = static_cast<float>(SDL_GetPerformanceFrequency());
}

void SDLBackendSystem::update_frame_time(float time_scale) {
    Uint64 now = SDL_GetPerformanceCounter();
    delta_time_ = static_cast<float>(now - last_) / freq_ * time_scale;
    last_ = now;
}

float SDLBackendSystem::delta_time() {
    return delta_time_;
}