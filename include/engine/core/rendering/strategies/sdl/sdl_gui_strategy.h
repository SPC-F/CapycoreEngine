#pragma once

#include <SDL3/sdl.h>

#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>

/**
 * @brief SDL implementation of the GUI rendering strategy.
 * 
 * SdlGuiStrategy is responsible for rendering GUI components using SDL.
 * It utilizes an SDL_Renderer to draw the associated GUI elements of the Component
 * onto the screen, applying necessary transformations and styles as specified by the GUI component.
 */
class SdlGuiStrategy final : public IRenderingStrategy {
private:
    SDL_Renderer& sdl_renderer_;
public:
    SdlGuiStrategy(SDL_Renderer& sdl_renderer);
    ~SdlGuiStrategy() override = default;
    void draw(Component& component) override;
};