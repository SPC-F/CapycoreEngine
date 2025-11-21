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

#include <SDL3/SDL.h>
#include <stdexcept>
#include <memory>

#include <engine/core/engine.h>
#include <engine/public/scene_service.h>
#include <engine/public/gameObject.h>
#include <engine/public/components/sprite.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/renderingService.h>

#include <engine/public/components/ui/ui_text.h>
#include <engine/public/components/ui/text.h>


void sdl_debug_loop() {
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

    }
}

void yes() {
    auto& engine = Engine::instance();

  
    auto& scene_service = engine.services->get_service<SceneService>().get();
    auto& asset_service = engine.services->get_service<AssetService>().get();
    auto& rendering_service = engine.services->get_service<RenderingService>().get();

    asset_service.load_from_resource("Fred_Sprites_REDO.png", "FRED", 12, 6);
    asset_service.register_texture("FRED", "TestTexture", 0);

    auto& scene = scene_service.add_scene("SomeScene");

    auto ui_text = std::make_unique<UIText>(
        "Hello, World!",
        "Roboto-Bold",
        "resources/fonts/roboto/Roboto-Bold.ttf",
        200.0f,
        50.0f,
        Point{0.5f, 0.5f},
        Point{0.5f, 0.5f},
        scene
    );
    ui_text->font_size(24);
    ui_text->color(Color{0, 0, 0, 255});

    scene.add_game_object(std::move(ui_text));

    // GameObject& game_object = scene.add_game_object("SomeGameObject");
    // game_object.transform().scale({3, 3, 1});
    // Sprite& sprite = game_object.add_component<Sprite>("", Color{255,0,0,255}, 0, 0, 1, 1);

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