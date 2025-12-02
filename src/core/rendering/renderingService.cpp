#include <map>

#include <engine/core/rendering/renderable.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>

RenderingService::RenderingService()
    : renderer_{new Renderer()},
      strategy_factory_{new SdlStrategyFactory(*renderer_)} {}
RenderingService::RenderingService(Renderer* renderer) : renderer_{renderer} {}

void RenderingService::draw(
    std::map<int, std::multimap<int, std::reference_wrapper<Renderable>>>& objects,
    Scene& scene) {
  renderer_->render(objects, scene);
}

Window& RenderingService::window() { return renderer_->window(); }

IRenderingStrategyFactory& RenderingService::rendering_strategy_factory()
    const {
  return *strategy_factory_;
}