#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>
#include <SDL3/SDL.h>

Engine::Engine() : services(std::make_unique<ServiceContainer>()) {
    services->register_service<AudioService>();
}

Engine& Engine::instance() {
    if (!engine_instance) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        engine_instance = std::unique_ptr<Engine, Deleter>(new Engine());
    }

    return *engine_instance;
}

void Engine::quit() {
    engine_instance.reset();
    SDL_Quit();
}

std::unique_ptr<Engine, Engine::Deleter> Engine::engine_instance = nullptr;