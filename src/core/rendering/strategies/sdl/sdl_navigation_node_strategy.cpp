#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/strategies/sdl/sdl_navigation_node_strategy.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/physics_service.h>
#include <engine/public/components/ai/navigation/navigation_node.h>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlNavigationNodeStrategy::SdlNavigationNodeStrategy(SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

void SdlNavigationNodeStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) return;
  if (!dynamic_cast<NavigationNode*>(&component)) return;

  NavigationNode& nav_node = dynamic_cast<NavigationNode&>(component);
  if (!nav_node.should_draw()) return;

  GameObject& parent = parent_opt->get();
  Vector3 world_pos = parent.transform().position();
  Vector3 cam_pos = camera.transform().position();

  float node_size = 10.0f;
  SDL_FPoint node_center;
  node_center.x = world_pos.x - cam_pos.x;
  node_center.y = world_pos.y - cam_pos.y;

  SDL_FRect rect;
  rect.w = node_size;
  rect.h = node_size;
  rect.x = node_center.x - node_size / 2.0f;
  rect.y = node_center.y - node_size / 2.0f;

  SDL_SetRenderDrawColor(&sdl_renderer_, 100, 0, 255, 255);
  SDL_RenderFillRect(&sdl_renderer_, &rect);

  // SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);
  // for (const auto& edge : nav_node.get_edges()) {
  //   NavigationNode& target_node = edge.target.get();
  //   auto target_parent_opt = target_node.parent();
  //   if (!target_parent_opt.has_value()) continue;

  //   Vector3 target_world_pos =
  //   target_parent_opt->get().transform().position();

  //   SDL_RenderLine(&sdl_renderer_, node_center.x, node_center.y,
  //                  target_world_pos.x - cam_pos.x,
  //                  target_world_pos.y - cam_pos.y);
  // }

  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 0, 255);
}
