#pragma once
#include <vector>
#include <engine/core/iEngineService.h>
#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/window.h>
#include <engine/core/rendering/strategies/irendering_strategy_factory.h>

/**
 * @brief Service responsible for rendering game objects to the window.
 * This service functions as a manager for the Renderer, and also as a face to work with the game window via a window property.
 */
class RenderingService : public IEngineService {
public:
    explicit RenderingService();
    explicit RenderingService(Renderer* renderer);
    ~RenderingService() override = default;
    /**
     * @brief Draws the provided game objects to the window using the renderer.
     * Delegates the drawing operation to the Renderer instance.
     * @param objects A vector of references to GameObject instances to be drawn.
     */
    void draw(const std::vector<std::reference_wrapper<GameObject>>& objects);
    /**
     * @brief Provides access to the game window.
     * @return A reference to the Window instance managed by the Renderer.
     */
    Window& window();

    [[nodiscard]] IRenderingStrategyFactory& rendering_strategy_factory() const;

private:
    friend class AssetService;
    const std::unique_ptr<Renderer> renderer_;
    const std::unique_ptr<IRenderingStrategyFactory> strategy_factory_;
};