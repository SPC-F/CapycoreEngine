#include <engine/core/engine.h>

Engine::Engine() :
    services(std::make_unique<ServiceContainer>()) {}