#pragma once
#include <engine/core/rendering/strategies/istrategy_factory.h>

class SdlStrategyFactory final : public IStrategyFactory {
public:
    SdlStrategyFactory();
    std::unique_ptr<IRenderingStrategy> get_strategy_for(RenderingStrategyType type) override;
};