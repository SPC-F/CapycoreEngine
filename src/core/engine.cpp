#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>

Engine::Engine() : services(std::make_unique<ServiceContainer>()) {
    services->register_service<AudioService>();
}