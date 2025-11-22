#pragma once
#include <memory>

#include <engine/core/rendering/rendererFlags.h>
#include <engine/public/gameObject.h>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/window.h>

class SDL_Renderer; // NOLINT
class SDL_Window; // NOLINT

using SdlRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
using SdlWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

/**
 * @brief Service responsible for rendering game objects to the screen
 *
 * The Renderer class encapsulates the SDL rendering context and provides methods to render
 * game objects, clear the screen, and manage the rendering window.
 */
class Renderer final : public IEngineService {
private:
    friend class RenderingManager;
    friend class AssetService;
    friend class SdlStrategyFactory;

    SdlRendererPtr sdl_renderer_;
    SdlWindowPtr sdl_window_;
    // I tried just storing a Window& here, but that caused issues because the can never be a proper init value...
    std::optional<Window> window_;
public:
    explicit Renderer();
    explicit Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags);

    /**
     * @brief Renders a collection of game objects to the screen.
     * Renders a given collection of game objects to the screen. Each game object is expected to have its own sprite, or it will be skipped. <br>
     * <br>
     * Mind that this method clears the entire screen and renders immediately on finish. If you want to batch multiple calls, fuse them into one.
     * @param objects
     */
    void render(std::vector<std::reference_wrapper<GameObject>>& objects) ;

    /**
     * @brief Clears the rendering target with the drawing color.
     * Clears the current rendering target with the drawing color set in the SDL renderer.
     */
    void clear() const;
    Window& window();
};
