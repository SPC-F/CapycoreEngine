#pragma once
#include <memory>

#include "SDL3/SDL_render.h"
#include <engine/public/gameObject.h>
#include <engine/core/iEngineService.h>

using SdlRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
class Renderer final : public IEngineService {
private:
    friend class RenderingManager;
    SdlRendererPtr _renderer;

public:
    explicit Renderer(SDL_Renderer* renderer);
    explicit Renderer(SdlRendererPtr renderer);
    void renderObject(GameObject& gameObj) const;
    void clear() const;
    void render() const;
};
