#pragma once
#include <vector>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/renderer.h>

class RenderingService : public IEngineService {
public:
    explicit RenderingService(Renderer* renderer);
    ~RenderingService() override = default;
    void draw(const std::vector<std::reference_wrapper<GameObject>>& objects);

    RenderingService& set_window_fullscreen();
    RenderingService& set_window_windowed();
    RenderingService& set_window_bordered();
    RenderingService& set_window_borderless();
    RenderingService& set_window_unresizable();
    RenderingService& set_window_resizable();

private:
    friend class AssetManager;
    const std::unique_ptr<Renderer> renderer_;
};