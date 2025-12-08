#include "engine/core/rendering/renderable.h"

#include <engine/public/util/layers.h>

#include "engine/core/engine.h"
#include "engine/core/rendering/renderingService.h"

Renderable::Renderable() : ordering_layer_{Layers::Default} {}
Renderable::Renderable(const int layer) : ordering_layer_{layer} {}

Renderable& Renderable::order_in_layer(int layer) {
  ordering_layer_ = layer;
  return *this;
}

[[nodiscard]] int Renderable::order_in_layer() const { return ordering_layer_; }

void Renderable::set_render_strategy(Component& component) {
  auto strateg = Engine::instance()
                     .services->get_service<RenderingService>()
                     .get()
                     .rendering_strategy_factory()
                     .create_strategy(component);

  render_strategy_ = std::move(strateg);
}

[[nodiscard]] IRenderingStrategy& Renderable::render_strategy() const {
  return *render_strategy_;
}

Renderable& Renderable::draw(bool enabled) noexcept {
  should_draw = enabled;
  return *this;
}

bool Renderable::draw() const noexcept { return should_draw; }