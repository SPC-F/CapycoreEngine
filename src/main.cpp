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

#include <engine/public/ui/ui_text.h>
#include <engine/public/ui/ui_button.h>

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

    asset_service.load_from_resource("ui/buttons_big.png", "UI", 4, 2);
    asset_service.register_texture("UI", "ButtonBlue", 0);
    asset_service.register_texture("UI", "ButtonRed", 1);
    asset_service.register_texture("UI", "ButtonYellow", 2);

    auto& scene = scene_service.add_scene("SomeScene");

    auto& title = scene.add_game_object<UIText>(
        "CapyBrawla",
        "ByteBounce",
        "resources/fonts/bytebounce/ByteBounce.ttf",
        200.0f,
        50.0f,
        Point{0.5f, 0.5f},
        Point{0.5f, 0.5f},
        scene
    );
    title.transform().position({310.0f, 100.0f, 0.0f});
    title.font_size(94);
    title.color(Color{0, 0, 0, 255});
 
    auto& start_button = scene.add_game_object<UIButton>(
        180.0f,
        60.0f,
        Point{0.5f, 0.5f},
        Point{0.5f, 0.5f},
        scene,
        "Start",
        "ByteBounce",
        "resources/fonts/bytebounce/ByteBounce.ttf",
        "ButtonBlue"
    );
    start_button.transform().position({310.0f, 200.0f, 0.0f});
    start_button.label_color(Color{255, 255, 255, 255});
    start_button.font_size(32);

    auto& tutorial_button = scene.add_game_object<UIButton>(
        180.0f,
        60.0f,
        Point{0.5f, 0.5f},
        Point{0.5f, 0.5f},
        scene,
        "Tutorial",
        "ByteBounce",
        "resources/fonts/bytebounce/ByteBounce.ttf",
        "ButtonYellow"
    );
    tutorial_button.transform().position({310.0f, 300.0f, 0.0f});
    tutorial_button.label_color(Color{255, 255, 255, 255});
    tutorial_button.font_size(32);

    auto& quit_button = scene.add_game_object<UIButton>(
        180.0f,
        60.0f,
        Point{0.5f, 0.5f},
        Point{0.5f, 0.5f},
        scene,
        "Quit",
        "ByteBounce",
        "resources/fonts/bytebounce/ByteBounce.ttf",
        "ButtonRed"
    );
    quit_button.transform().position({310.0f, 400.0f, 0.0f});
    quit_button.label_color(Color{255, 255, 255, 255});
    quit_button.font_size(32);


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