#include <SDL3/SDL.h>
#include <engine/audio/audio_service.h>
#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/system/system_service.h>
#include <engine/input/input_manager.h>
#include <engine/input/input_system.h>
#include <engine/input/strategy/sdl_input_strategy.h>
#include <engine/physics/physics_service.h>
#include <engine/public/scene_service.h>
#include <engine/storage/simple_storage.h>
#include <engine/network/multiplayer_service.h>
#include <engine/public/prefab_service.h>

#include <memory>

#include "engine/public/gameplay_speed_service.h"

Engine::Engine() : services(std::make_unique<ServiceContainer>()) {
  services->register_service<SceneService>();
  services->register_service<RenderingService>();
  services->register_service<SystemService>();
  services->register_service<AudioService>();
  services->register_service<AssetService>();
  services->register_service<MultiplayerService>();
  services->register_service<PrefabService>();
}

Engine& Engine::instance() {
  if (!engine_instance_) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    engine_instance_ = std::unique_ptr<Engine, Deleter>(new Engine());
  }

  return *engine_instance_;
}

void Engine::initialize() {
  const auto& services = instance().services;

  services->register_service<PhysicsService>();
  services->register_service<SceneService>();
  auto& input_manager = services->register_service<InputManager>();

  // Setup SDL components
  auto input_system = std::make_unique<InputSystem>();
  input_system->set_input(std::make_unique<SDLInputStrategy>());
  input_manager.set_provider(std::move(input_system));

  services->register_service<GameplaySpeedService>(input_manager.provider());
  SimpleStorage::instance().load();
}

void Engine::quit() {
  engine_instance_.reset();
  SDL_Quit();
}

std::unique_ptr<Engine, Engine::Deleter> Engine::engine_instance_ = nullptr;