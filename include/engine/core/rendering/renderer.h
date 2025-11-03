#pragma once
#include <memory>

#include "SDL3/SDL_render.h"
#include <engine/public/gameObject.h>
#include <engine/core/iEngineService.h>

using SdlRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
using SdlWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

class Renderer final : public IEngineService {
private:
    friend class RenderingManager;
    SdlRendererPtr renderer_;
    SdlWindowPtr window_;

    explicit Renderer(SDL_Renderer* renderer, SDL_Window* window);
    explicit Renderer(SdlRendererPtr renderer, SdlWindowPtr window);
public:
    void update();
    void render(const std::vector<std::reference_wrapper<GameObject>>& objects) const;
    void clear() const;
};
