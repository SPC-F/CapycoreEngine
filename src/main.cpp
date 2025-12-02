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

#include <SDL3/SDL.h>
#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/input/input_manager.h>
#include <engine/public/behavior.h>
#include <engine/public/camera.h>
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

class PlayerBehavior : public Behavior {
 public:
  void on_update(float delta_time) override {
    auto rb_opt = get_component<Rigidbody2D>();
    if (!rb_opt.has_value()) return;

    auto& rb = rb_opt->get();
    auto& provider = Engine::instance()
                         .services->get_service<InputManager>()
                         .get()
                         .provider();

    // Horizontal input
    float horizontal = 0.0f;
    if (provider.is_key_pressed(KeyCode::d) || provider.is_key_held(KeyCode::d))
      horizontal += 1.0f;
    if (provider.is_key_pressed(KeyCode::a) || provider.is_key_held(KeyCode::a))
      horizontal -= 1.0f;

    float speed = 10.0f;

    // Apply horizontal movement
    Vector3 velocity = rb.velocity();
    velocity.x = horizontal * speed;  // only horizontal
    rb.velocity(velocity);

    // Jumping
    float jump_strength = 10.0f;
    if (provider.is_key_pressed(KeyCode::w)) {
      rb.apply_force({0.0f, -jump_strength, 0.0f});
      std::cout << "Jump!" << std::endl;
    }
  }
};

void yes() {
  auto& engine = Engine::instance();
  engine.initialize();

  auto& scene_service = engine.services->get_service<SceneService>().get();
  auto& asset_service = engine.services->get_service<AssetService>().get();
  auto& rendering_service =
      engine.services->get_service<RenderingService>().get();

  asset_service.load_from_resource("Fred_Sprites_REDO.png", "FRED", 12, 6);
  asset_service.register_texture("FRED", "fred_default", 0);

  asset_service.load_from_resource("Mud_Tiles.png", "MUD", 8, 8);
  asset_service.register_texture("MUD", "mud_tile_1", 0);

  auto& scene = scene_service.add_scene("SomeScene");
  scene.add_game_object<Camera>(scene, Color{0, 0, 0, 255}, 1.0f, true);

  // test playform with player
  auto& player = scene.add_game_object("Player");
  player.transform().position({100.0f, 100.0f, 0.0f});
  player.add_component<Sprite>("fred_default", Color{255, 255, 255, 255}, 0, 0,
                               0, 0);
  player.add_component<Rigidbody2D>(BodyType2D::Dynamic, 30.0f, true, 1.0f);
  player.add_component<BoxCollider2D>(0.5f, 0.2f, 16.0f, 16.0f,
                                      Point{8.0f, 0.0f});
  player.add_component<CircleCollider2D>(0.5f, 0.5f, 8.0f, Point{16.0f, 16.0f});
  player.add_component<BehaviorScript>(std::make_unique<PlayerBehavior>());

  auto& platform = scene.add_game_object("Platform");
  platform.transform().position({100.0f, 300.0f, 0.0f});
  platform.transform().scale({5.0f, 1.0f, 1.0f});
  platform.add_component<Sprite>("mud_tile_1", Color{255, 255, 255, 255}, 0, 0,
                                 0, 0);
  platform.add_component<Rigidbody2D>(BodyType2D::Static, 0.0f, false, 0.0f);
  platform.add_component<BoxCollider2D>(0.5f, 0.2f, 16.0f * 5.0f, 16.0f,
                                        Point{0.0f, 0.0f});

  scene_service.load_scene("SomeScene");

  // sdl_debug_loop();

  Engine::quit();
}

int main() {
  tracy_init();
  yes();
  tracy_shutdown();
  return 0;
}