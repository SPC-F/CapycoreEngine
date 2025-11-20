#include <engine/core/rendering/renderingService.h>
#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>

RenderingService::RenderingService()
    : renderer_{new Renderer()},
    strategy_factory_{new SdlStrategyFactory(*renderer_)} {
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

IRenderingStrategyFactory& RenderingService::rendering_strategy_factory() const {
    return *strategy_factory_;
}