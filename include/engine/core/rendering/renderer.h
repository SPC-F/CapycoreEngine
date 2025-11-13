#pragma once
#include <memory>

#include <SDL3/SDL_render.h>
#include <engine/core/rendering/rendererFlags.h>
#include <engine/public/gameObject.h>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/window.h>

using SdlRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
using SdlWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

class Renderer final : public IEngineService {
private:
    friend class RenderingManager;
    friend class AssetService;
    SdlRendererPtr sdl_renderer_;
    SdlWindowPtr sdl_window_;
    // I tried just storing a Window& here, but that caused issues because the can never be a proper init value...
    std::optional<Window> window_;
public:
    explicit Renderer();
    explicit Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags);

    void render(const std::vector<std::reference_wrapper<GameObject>>& objects) const;
    void clear() const;
    Window& window();
};
