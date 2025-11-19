#include <engine/core/rendering/renderingService.h>
#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>

RenderingService::RenderingService()
    : renderer_{new Renderer()},
    strategy_factory_{new SdlStrategyFactory()} {
}

RenderingService::RenderingService(Renderer* renderer)
    : renderer_{renderer}{
}

void RenderingService::draw(const std::vector<std::reference_wrapper<GameObject>>& objects) {
    renderer_->render(objects);
}

Window& RenderingService::window() {
    return renderer_->window();
}

std::unique_ptr<IRenderingStrategy> RenderingService::create_strategy_for_type(RenderingStrategyType type) const {
    return strategy_factory_->create_strategy_for(type);
}