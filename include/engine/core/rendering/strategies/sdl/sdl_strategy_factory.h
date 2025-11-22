#pragma once

#include <engine/core/rendering/strategies/irendering_strategy_factory.h>

/**
 * @brief Factory for creating SDL rendering strategies.
 * 
 * SdlStrategyFactory is responsible for producing instances of SDL-based rendering strategies
 * for various components. It implements the IRenderingStrategyFactory interface and utilizes
 * the provided Renderer to create appropriate strategies.
 */
class SdlStrategyFactory final : public IRenderingStrategyFactory {
public:
    SdlStrategyFactory(Renderer& renderer);
    std::unique_ptr<IRenderingStrategy> create_strategy(Component& component) override;
};