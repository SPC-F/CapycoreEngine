#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>
#include <engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h>
#include "engine/core/rendering/strategies/sdl/sdl_gui_strategy.h"

std::unique_ptr<IRenderingStrategy> SdlStrategyFactory::create_strategy_for(const RenderingStrategyType type) {
    switch (type) {
        case RenderingStrategyType::GUI: {
            return std::make_unique<SdlSpriteStrategy>();
        } break;
        case RenderingStrategyType::SPRITE: {
            return std::make_unique<SdlGuiStrategy>();
        } break;
        default: {
            throw std::runtime_error("Unknown strategy type provided");
        } break;
    }
}