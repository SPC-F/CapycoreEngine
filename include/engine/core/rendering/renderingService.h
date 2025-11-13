#pragma once
#include <vector>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/window.h>

class RenderingService : public IEngineService {
public:
    explicit RenderingService();
    explicit RenderingService(Renderer* renderer);
    ~RenderingService() override = default;
    void draw(const std::vector<std::reference_wrapper<GameObject>>& objects);
    Window& window();

private:
    friend class AssetService;
    const std::unique_ptr<Renderer> renderer_;
};