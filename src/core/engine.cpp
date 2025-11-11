#include <engine/core/engine.h>
#include <engine/core/assetManager.h>

Engine::Engine() :
    services(std::make_unique<ServiceContainer>()) {
    services->register_service<AssetManager>();
}