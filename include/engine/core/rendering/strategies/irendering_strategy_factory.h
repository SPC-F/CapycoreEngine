#pragma once

#include <memory>

#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>

class Component;

/**
 * @brief Interface for factories that create rendering strategies.
 * 
 * IRenderingStrategyFactory defines a contract for factories that produce
 * IRenderingStrategy instances based on the provided Component. Each factory
 * is associated with a Renderer which may be used in the creation of strategies.
 */
class IRenderingStrategyFactory {
protected:
    Renderer& renderer_;
public:
    explicit IRenderingStrategyFactory(Renderer& renderer);
    virtual ~IRenderingStrategyFactory() = default;

    virtual std::unique_ptr<IRenderingStrategy> create_strategy(Component& component) = 0;
};
