#pragma once

#include <memory>
#include <engine/core/serviceContainer.h>

/**
 * @brief Central engine class managing core services and lifecycle.
 * 
 * Provides access to global services via the ServiceContainer.
 * Responsible for engine initialization and shutdown.
 */
class Engine {
private:
    struct Deleter {
        void operator()(Engine* e) const { delete e; }
    };

    Engine();

    static std::unique_ptr<Engine, Deleter> engine_instance_; // NOLINT(readability-identifier-naming)
public:
    /**
     * @brief Retrieves the singleton engine instance.
     * 
     * Initializes the engine if not already done and should be the first call made.
     * This only initializes the CORE services; call initialize() to set up all services.
     */
    static Engine& instance();

    /**
     * @brief Initializes the engine and its core services.
     * 
     * Must be called before using any engine functionality as it sets up necessary systems.
     * This registers all except the core services, as they utilize the engine instance.
     */
    static void initialize();

    /**
     * @brief Shuts down the engine and cleans up resources.
     * 
     * Should be called at application exit to ensure proper resource management.
     */
    static void quit();

    const std::unique_ptr<ServiceContainer> services;
};
