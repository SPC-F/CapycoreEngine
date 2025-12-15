#include <engine/core/rendering/strategies/sdl/sdl_box_collider_2d_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_circle_collider_2d_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_image_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_navigation_node_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_pathfinding_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h>
#include <engine/core/rendering/strategies/sdl/sdl_strategy_factory.h>
#include <engine/core/rendering/strategies/sdl/sdl_text_strategy.h>
#include <engine/public/components/ai/navigation/navigation_node.h>
#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/components/colliders/box_collider_2d.h>
#include <engine/public/components/colliders/circle_collider_2d.h>
#include <engine/public/components/sprite.h>
#include <engine/public/components/ui/image.h>
#include <engine/public/components/ui/text.h>

SdlStrategyFactory::SdlStrategyFactory(Renderer& renderer)
    : IRenderingStrategyFactory(renderer) {}

std::unique_ptr<IRenderingStrategy> SdlStrategyFactory::create_strategy(
    Component& component) {
  if (auto const* sprite = dynamic_cast<Sprite*>(&component)) {
    return std::make_unique<SdlSpriteStrategy>(*renderer_.sdl_renderer_);
  }

  if (auto const* text = dynamic_cast<Text*>(&component)) {
    return std::make_unique<SdlTextStrategy>(*renderer_.sdl_renderer_);
  }

  if (auto const* image = dynamic_cast<Image*>(&component)) {
    return std::make_unique<SdlImageStrategy>(*renderer_.sdl_renderer_);
  }

  if (auto const* box_collider = dynamic_cast<BoxCollider2D*>(&component)) {
    return std::make_unique<SdlBoxCollider2DStrategy>(*renderer_.sdl_renderer_);
  }

  if (auto const* circle_collider =
          dynamic_cast<CircleCollider2D*>(&component)) {
    return std::make_unique<SdlCircleCollider2DStrategy>(
        *renderer_.sdl_renderer_);
  }

  if (auto const* navigation_node = dynamic_cast<NavigationNode*>(&component)) {
    return std::make_unique<SdlNavigationNodeStrategy>(
        *renderer_.sdl_renderer_);
  }

  if (auto const* pathfinding = dynamic_cast<Pathfinding*>(&component)) {
    return std::make_unique<SdlPathFindingStrategy>(*renderer_.sdl_renderer_);
  }

  return {};
}