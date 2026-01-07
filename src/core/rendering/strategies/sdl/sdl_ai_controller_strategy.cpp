#include <engine/core/engine.h>
#include <engine/core/rendering/strategies/sdl/sdl_ai_controller_strategy.h>
#include <engine/public/components/ai/ai_controller.h>

SdlAIControllerStrategy::SdlAIControllerStrategy(SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

void SdlAIControllerStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) return;
  if (!dynamic_cast<AIController*>(&component)) return;

  AIController& ai_controller = dynamic_cast<AIController&>(component);
  if (!ai_controller.should_draw()) return;

  const auto& path = ai_controller.get_path();

  for (size_t i = 0; i < path.size(); ++i) {
    GameObject& node = path[i].get();
    Vector3 world_pos = node.transform().position();
    Vector3 cam_pos = camera.transform().position();

    const auto zoom = camera.zoom();

    const auto half_screen_width = camera.get_screen_width() * 0.5f;
    const auto half_screen_height = camera.get_screen_height() * 0.5f;

    float node_size = 8.0f;
    SDL_FRect rect;
    rect.w = node_size;
    rect.h = node_size;
    rect.x =
        (world_pos.x - cam_pos.x) * zoom + half_screen_width - node_size / 2.0f;
    rect.y = (world_pos.y - cam_pos.y) * zoom + half_screen_height -
             node_size / 2.0f;

    SDL_SetRenderDrawColor(&sdl_renderer_, 0, 255, 0, 255);
    SDL_RenderFillRect(&sdl_renderer_, &rect);

    if (i + 1 < path.size()) {
      GameObject& next_node = path[i + 1].get();
      Vector3 next_world_pos = next_node.transform().position();

      SDL_SetRenderDrawColor(&sdl_renderer_, 0, 200, 0, 255);
      SDL_RenderLine(
          &sdl_renderer_, (world_pos.x - cam_pos.x) * zoom + half_screen_width,
          (world_pos.y - cam_pos.y) * zoom + half_screen_height,
          (next_world_pos.x - cam_pos.x) * zoom + half_screen_width,
          (next_world_pos.y - cam_pos.y) * zoom + half_screen_height);
    }
  }
}
