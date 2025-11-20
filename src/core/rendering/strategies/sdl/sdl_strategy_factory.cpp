#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>
#include <engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_gui_strategy.h>
#include <engine/public/components/sprite.h>

SdlStrategyFactory::SdlStrategyFactory(Renderer &renderer): IRenderingStrategyFactory(renderer) {

}

std::unique_ptr<IRenderingStrategy> SdlStrategyFactory::create_sprite_strategy(Sprite& sprite, GameObject& parent) {
    return std::make_unique<SdlSpriteStrategy>(sprite, parent, renderer_.sdl_renderer_.get());
}

std::unique_ptr<IRenderingStrategy> SdlStrategyFactory::create_gui_strategy() {
    return {};
}
