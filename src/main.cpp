#include <engine/util/memory.h>

// Macro's for tracy based on the CMake option
namespace {
#ifdef TRACY_ENABLE
inline void tracy_init() { tracy_memory_init(); }
inline void tracy_shutdown() { tracy_memory_shutdown(); }
#else
inline void tracy_init() {}
inline void tracy_shutdown() {}
#endif
}  // namespace

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/input/input_manager.h>
#include <engine/physics/physics_service.h>
#include <engine/public/behavior.h>
#include <engine/public/camera.h>
#include <engine/public/components/ai/navigation/navigation_graph.h>
#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/components/behaviorscript.h>
#include <engine/public/components/colliders/box_collider_2d.h>
#include <engine/public/components/colliders/circle_collider_2d.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/components/sprite.h>
#include <engine/public/components/ui/text.h>
#include <engine/public/gameObject.h>
#include <engine/public/scene_service.h>
#include <engine/public/ui/interactable/ui_button.h>
#include <engine/public/ui/interactable/ui_input.h>
#include <engine/public/ui/ui_text.h>
#include <engine/public/util/color.h>
#include <engine/public/util/point.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

void run() {
  auto& engine = Engine::instance();
  engine.initialize();

  // construct a maze of blank textures in a 10x10 grid
  // const std::vector<std::vector<int>> maze = {
  //   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //   {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  //   {2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
  //   {0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0},
  //   {0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 3},
  //   {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  //   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  // };

  const std::vector<std::vector<int>> maze = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 0, 1, 0, 0, 0, 3, 0, 1, 0},
      {0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
      {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0},
      {0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
      {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
      {0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };

  auto& physics_service = engine.services->get_service<PhysicsService>().get();
  physics_service.debug_mode(true);

  auto& scene_service = engine.services->get_service<SceneService>().get();
  auto& scene = scene_service.add_scene("MazeScene");
  scene.add_game_object<Camera>(scene, Color{0, 0, 0, 255}, 1.0f, true);

  auto& tile_map_parent = scene.add_game_object("TileMapParent");
  auto& navigation_graph =
      tile_map_parent.add_component<NavigationGraph>(32, 10);
  auto& pathfinding = tile_map_parent.add_component<Pathfinding>();

  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 1) {
        auto& wall = scene.add_game_object("Wall_" + std::to_string(x) + "_" +
                                           std::to_string(y));

        wall.transform().position(
            {static_cast<float>(x * 32), static_cast<float>(y * 32), 0.0f});
        wall.transform().scale({1.0f, 1.0f, 1.0f});
        wall.add_component<Sprite>("", Color{255, 255, 255, 255}, 0, 0, 0, 0);
        wall.add_component<Rigidbody2D>(BodyType2D::Static, 0.0f, false, 0.0f);
        wall.add_component<BoxCollider2D>(0.5f, 0.2f, 32.0f, 32.0f,
                                          Point{0.0f, 0.0f});
        tile_map_parent.add_child(wall);
      }

      if (maze[y][x] == 2) {
        auto& start = scene.add_game_object("StartPoint");

        start.transform().position(
            {static_cast<float>(x * 32), static_cast<float>(y * 32), 0.0f});
        start.transform().scale({0.5f, 0.5f, 1.0f});
        start.add_component<Sprite>("", Color{0, 255, 0, 255}, 0, 0, 0, 0);
        pathfinding.set_origin(start.transform().position());
      }

      if (maze[y][x] == 3) {
        auto& end = scene.add_game_object("EndPoint");

        end.transform().position(
            {static_cast<float>(x * 32), static_cast<float>(y * 32), 0.0f});
        end.transform().scale({1.0f, 1.0f, 1.0f});
        end.add_component<Sprite>("", Color{255, 0, 0, 255}, 0, 0, 0, 0);

        pathfinding.set_target(end.transform().position());
      }
    }
  }

  navigation_graph.generate_graph();
  pathfinding.generate_path_to_target();
  // pathfinding.generate_path_to_position({start_x, start_y, 0.0f},
  //                                       {8 * 32, 3 * 32, 0.0f});

  scene_service.load_scene("MazeScene");

  engine.quit();
}

int main() {
  tracy_init();
  run();
  tracy_shutdown();
  return 0;
}