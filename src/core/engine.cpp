#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>
#include <engine/core/assetManager.h>

Engine::Engine() :
    services(std::make_unique<ServiceContainer>()) {
    services->register_service<AssetManager>();
    services->register_service<AudioService>();
}