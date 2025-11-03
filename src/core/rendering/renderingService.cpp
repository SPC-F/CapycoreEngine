#include <engine/core/rendering/renderingService.h>

RenderingService::RenderingService(Renderer* renderer)
    : renderer_{renderer}{
}

void RenderingService::draw(const std::vector<std::reference_wrapper<GameObject>>& objects) {
    renderer_->render(objects);
}