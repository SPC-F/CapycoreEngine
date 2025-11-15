#include <engine/core/rendering/renderingService.h>

RenderingService::RenderingService()
    : renderer_{new Renderer()} {
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