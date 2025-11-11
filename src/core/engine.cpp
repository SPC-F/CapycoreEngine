#include <engine/core/engine.h>

#include <engine/audio/audio_service.h>
#include <engine/physics/physics_service.h>

Engine::Engine() : services(std::make_unique<ServiceContainer>()) {
    services->register_service<AudioService>();
    services->register_service<PhysicsService>();
}