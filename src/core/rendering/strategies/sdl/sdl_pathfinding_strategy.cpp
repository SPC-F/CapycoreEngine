#include <engine/core/engine.h>
#include <engine/core/rendering/strategies/sdl/sdl_pathfinding_strategy.h>
#include <engine/public/components/ai/navigation/pathfinding.h>

SdlPathFindingStrategy::SdlPathFindingStrategy(SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

void SdlPathFindingStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) return;
  if (!dynamic_cast<Pathfinding*>(&component)) return;

  Pathfinding& pathfinding = dynamic_cast<Pathfinding&>(component);
  if (!pathfinding.should_draw()) return;

  const auto& path = pathfinding.get_path();

  for (size_t i = 0; i < path.size(); ++i) {
    GameObject& node = path[i].get();
    Vector3 world_pos = node.transform().position();
    Vector3 cam_pos = camera.transform().position();

    float node_size = 8.0f;
    SDL_FRect rect;
    rect.w = node_size;
    rect.h = node_size;
    rect.x = world_pos.x - cam_pos.x - node_size / 2.0f;
    rect.y = world_pos.y - cam_pos.y - node_size / 2.0f;

    SDL_SetRenderDrawColor(&sdl_renderer_, 0, 255, 0, 255);
    SDL_RenderFillRect(&sdl_renderer_, &rect);

    if (i + 1 < path.size()) {
      GameObject& next_node = path[i + 1].get();
      Vector3 next_world_pos = next_node.transform().position();

      SDL_SetRenderDrawColor(&sdl_renderer_, 0, 200, 0, 255);
      SDL_RenderLine(&sdl_renderer_, world_pos.x - cam_pos.x,
                     world_pos.y - cam_pos.y, next_world_pos.x - cam_pos.x,
                     next_world_pos.y - cam_pos.y);
    }
  }
}
