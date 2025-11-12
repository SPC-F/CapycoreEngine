#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>
#include <engine/core/assetManager.h>
#include <engine/core/rendering/renderingService.h>

Engine::Engine() :
    services(std::make_unique<ServiceContainer>()) {
    services->register_service<AssetManager>();
    services->register_service<RenderingService>();
    services->register_service<AudioService>();
}