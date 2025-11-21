#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>

#include <engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_text_strategy.h>

#include <engine/public/components/ui/text.h>
#include <engine/public/components/sprite.h>

SdlStrategyFactory::SdlStrategyFactory(Renderer &renderer): IRenderingStrategyFactory(renderer) {

}

std::unique_ptr<IRenderingStrategy> SdlStrategyFactory::create_strategy(Component& component) {
    if (auto* sprite = dynamic_cast<Sprite*>(&component)) {
        return std::make_unique<SdlSpriteStrategy>(*renderer_.sdl_renderer_);
    }

    if (auto* text = dynamic_cast<Text*>(&component)) {
        return std::make_unique<SdlTextStrategy>(*renderer_.sdl_renderer_);
    }

    return {};
}