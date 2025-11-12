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
#include <engine/core/engine.h>
#include <engine/core/assetManager.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/public/gameObject.h>
#include <engine/public/component.h>
#include <engine/public/components/sprite.h>

void run_game_loop() {
    const std::string file_name = "Fred_Sprites_REDO.png";
    const std::string texture_name = "pingu";
    Engine& engine = Engine::instance();
    Scene scene = Scene();

    auto& renderService = engine.services->get_service<RenderingService>().get();

    auto& assetManager = engine.services->get_service<AssetManager>().get();
    assetManager.load_from_resource(file_name, texture_name, 12, 6);
    assetManager.register_texture(texture_name, "pingu1", 0);

    GameObject pingu = GameObject(scene);
    pingu.add_component<Sprite>(pingu, "pingu1", Color(), 0, 0, 0, 0);

    bool run = true;
    while (run) {
        SDL_Event e {};
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT: {
                    run = false;
                }
                    break;
                default: {
                    break;
                }
            }
        }

        renderService.draw({pingu});
    }
}

int main() {
    tracy_init();

    run_game_loop();

    tracy_shutdown();
    return 0;
}