#pragma once
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/core/rendering/strategies/rendering_strategy_type.h>
#include <memory>

class IStrategyFactory {
public:
    virtual ~IStrategyFactory() = default;
    virtual std::unique_ptr<IRenderingStrategy> get_strategy_for(RenderingStrategyType type) = 0;
};
