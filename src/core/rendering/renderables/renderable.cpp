#include <engine/core/rendering/renderables/renderable.h>

Renderable::Renderable(std::unique_ptr<IRenderingStrategy> render_strategy) :
    render_strategy_(std::move(render_strategy)){
}

[[nodiscard]] IRenderingStrategy& Renderable::render_strategy() const {
    return *render_strategy_;
}