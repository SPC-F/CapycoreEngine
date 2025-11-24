#include <engine/core/rendering/renderingService.h>
#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>

RenderingService::RenderingService()
    : renderer_{new Renderer()},
    strategy_factory_{new SdlStrategyFactory(*renderer_)} {
}

RenderingService::RenderingService(Renderer* renderer)
    : renderer_{renderer}{
}

void RenderingService::draw(std::vector<std::reference_wrapper<GameObject>>& objects) {
    renderer_->render(objects);
}

Window& RenderingService::window() {
    return renderer_->window();
}

IRenderingStrategyFactory& RenderingService::rendering_strategy_factory() const {
    return *strategy_factory_;
}

void RenderingService::init_frame_timer() {
    renderer_->init_frame_timer();
}

void RenderingService::update_frame_time(float time_scale) {
    renderer_->update_frame_time(time_scale);
}

float RenderingService::delta_time() const {
    return renderer_->delta_time();
}