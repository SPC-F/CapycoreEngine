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
#include <engine/public/scene_service.h>
#include <engine/public/gameObject.h>
#include <engine/public/components/sprite.h>
#include <engine/core/rendering/assetService.h>
#include "engine/public/components/animator.h"

void yes() {
  const auto& engine = Engine::instance();
  engine.initialize();

  auto& scene_service = engine.services->get_service<SceneService>().get();
  auto& asset_service = engine.services->get_service<AssetService>().get();

  asset_service.load_from_resource("Fred_Sprites_REDO.png", "FRED", 12, 6);
  asset_service.register_texture("FRED", "BaseTexture", 0);
  asset_service.create_spritesheet_for("FRED", "TestAnimation", 0, 12);

  auto& scene = scene_service.add_scene("SomeScene");
  Camera& camera = scene.add_game_object<Camera>(scene, Color(), 1.0f, true);

  GameObject& game_object = scene.add_game_object("SomeGameObject");
  game_object.transform().scale({3, 3, 1});
  auto& sprite = game_object.add_component<Sprite>("BaseTexture", Color(), 0, 0, 1, 1);
  auto& animation = game_object.add_component<Animator>("TestAnimation", 125);

  game_object.layer(5);

  animation.play(true);

  try {
    scene_service.load_scene("SomeScene");
  } catch (const std::exception& e) {

  }

  Engine::quit();
}

int main() {
  tracy_init();
  yes();
  tracy_shutdown();
  return 0;
}