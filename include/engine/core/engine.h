#pragma once

#include <memory>
#include <engine/core/serviceContainer.h>

class Engine {
private:
    struct Deleter {
        void operator()(Engine* e) const { delete e; }
    };

    Engine();

    static std::unique_ptr<Engine, Deleter> engine_instance_; // NOLINT(readability-identifier-naming)
public:
    static Engine& instance();
    static void quit();

    const std::unique_ptr<ServiceContainer> services;
};
