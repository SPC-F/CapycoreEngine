#include <engine/core/rendering/renderingService.h>

RenderingService::RenderingService(Renderer* renderer)
    : renderer_{renderer}{
}

void RenderingService::draw(const std::vector<std::reference_wrapper<GameObject>>& objects) {
    renderer_->render(objects);
}

RenderingService& RenderingService::set_window_fullscreen() {
    renderer_->set_window_fullscreen();
    return *this;
}
RenderingService& RenderingService::set_window_windowed() {
    renderer_->set_window_windowed();
    return *this;
}
RenderingService& RenderingService::set_window_bordered() {
    renderer_->set_window_bordered();
    return *this;
}
RenderingService& RenderingService::set_window_borderless() {
    renderer_->set_window_borderless();
    return *this;
}
RenderingService& RenderingService::set_window_unresizable() {
    renderer_->set_window_unresizable();
    return *this;
}
RenderingService& RenderingService::set_window_resizable() {
    renderer_->set_window_resizable();
    return *this;
}