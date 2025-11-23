#include "engine/core/rendering/renderable.h"

#include "engine/core/engine.h"
#include "engine/core/rendering/renderingService.h"

void Renderable::set_render_strategy(Component& component) {
    auto strateg = Engine::instance()
            .services
            ->get_service<RenderingService>()
            .get()
            .rendering_strategy_factory()
            .create_strategy(component);
            
    render_strategy_ = std::move(strateg);
}

[[nodiscard]] IRenderingStrategy& Renderable::render_strategy() const {
    return *render_strategy_;
}