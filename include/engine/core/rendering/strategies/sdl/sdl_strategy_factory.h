#pragma once
#include <engine/core/rendering/strategies/istrategy_factory.h>

class SdlStrategyFactory final : public IStrategyFactory {
public:
    SdlStrategyFactory() = default;
    std::unique_ptr<IRenderingStrategy> create_strategy_for(RenderingStrategyType type) override;
};