#pragma once
#include <vector>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/renderer.h>

class SDL_Renderer;
class RenderingService : public IEngineService {
public:
    explicit RenderingService(Renderer* renderer);
    ~RenderingService() override = default;
    void draw(const std::vector<std::reference_wrapper<GameObject>>& objects);

private:
    friend class AssetManager;
    const std::unique_ptr<Renderer> renderer_; // owns SDL_Renderer*
};