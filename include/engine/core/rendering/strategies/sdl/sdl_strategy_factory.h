#pragma once
#include <engine/core/rendering/strategies/irendering_strategy_factory.h>

class SdlStrategyFactory final : public IRenderingStrategyFactory {
public:
    SdlStrategyFactory(Renderer& renderer);
    std::unique_ptr<IRenderingStrategy> create_sprite_strategy(Sprite& sprite, GameObject& parent) override;
    std::unique_ptr<IRenderingStrategy> create_gui_strategy() override;
};