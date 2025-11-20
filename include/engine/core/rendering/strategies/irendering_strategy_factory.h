#pragma once
#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <memory>

class Sprite;

class IRenderingStrategyFactory {
protected:
    Renderer& renderer_;
public:
    explicit IRenderingStrategyFactory(Renderer& renderer);
    virtual ~IRenderingStrategyFactory() = default;

    virtual std::unique_ptr<IRenderingStrategy> create_sprite_strategy(Sprite& sprite, GameObject& parent) = 0;
    virtual std::unique_ptr<IRenderingStrategy> create_gui_strategy() = 0;
};
