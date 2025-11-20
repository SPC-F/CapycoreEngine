#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/physics/physics_service.h>
#include <engine/public/scene_service.h>
#include <SDL3/SDL.h>

Engine::Engine() : services(std::make_unique<ServiceContainer>()) {
    services->register_service<AssetService>();
    services->register_service<RenderingService>();
    services->register_service<AudioService>();
    services->register_service<PhysicsService>();
    services->register_service<SceneService>();
}

Engine& Engine::instance() {
    if (!engine_instance_) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        engine_instance_ = std::unique_ptr<Engine, Deleter>(new Engine());
    }

    return *engine_instance_;
}

void Engine::quit() {
    engine_instance_.reset();
    SDL_Quit();
}

std::unique_ptr<Engine, Engine::Deleter> Engine::engine_instance_ = nullptr;
