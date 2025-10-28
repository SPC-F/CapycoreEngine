#pragma once
#include <memory>

#include <engine/core/serviceContainer.h>

class Engine {
private:
    Engine();
public:
    static Engine& instance() {
        static Engine engine {};
        return engine;
    }

    const std::unique_ptr<ServiceContainer> services;
};
