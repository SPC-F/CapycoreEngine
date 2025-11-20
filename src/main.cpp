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
}

#include <engine/core/engine.h>
#include <engine/public/scene_service.h>
#include <engine/public/gameObject.h>
#include <engine/public/components/sprite.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>

void yes() {
    auto& engine = Engine::instance();
    auto& scene_service = engine.services->get_service<SceneService>().get();
    auto& asset_service = engine.services->get_service<AssetService>().get();
    auto& rendering_service = engine.services->get_service<RenderingService>().get();

    asset_service.load_from_resource("Fred_Sprites_REDO.png", "FRED", 12, 6);
    asset_service.register_texture("FRED", "TestTexture", 0);

    auto& scene = scene_service.add_scene("SomeScene");
    GameObject& game_object = scene.add_game_object("SomeGameObject");
    game_object.transform().scale({3, 3, 1});
    Sprite& sprite = game_object.add_component<Sprite>("TestTexture", Color(), 0, 0, 1, 1);

    scene_service.load_scene("SomeScene");

    Engine::quit();
}

int main() {
    tracy_init();
    yes();
    tracy_shutdown();
    return 0;
}