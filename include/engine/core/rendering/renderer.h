#pragma once
#include <memory>

#include <SDL3/SDL_render.h>
#include <engine/core/rendering/RendererFlags.h>
#include <engine/public/gameObject.h>
#include <engine/core/iEngineService.h>

using SdlRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
using SdlWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

class Renderer final : public IEngineService {
private:
    friend class RenderingManager;
    friend class AssetManager;
    SdlRendererPtr renderer_;
    SdlWindowPtr window_;
public:
    explicit Renderer();
    explicit Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags);

    void update();
    void render(const std::vector<std::reference_wrapper<GameObject>>& objects) const;
    void clear() const;

    Renderer& set_window_fullscreen();
    Renderer& set_window_windowed();
    Renderer& set_window_bordered();
    Renderer& set_window_borderless();
    Renderer& set_window_unresizable();
    Renderer& set_window_resizable();
};
