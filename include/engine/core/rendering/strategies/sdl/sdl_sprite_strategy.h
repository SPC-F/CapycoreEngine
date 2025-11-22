#pragma once

#include <SDL3/SDL.h>

#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>

/**
 * @brief SDL implementation of the sprite rendering strategy.
 * 
 * SdlSpriteStrategy is responsible for rendering Sprite components using SDL.
 * It utilizes an SDL_Renderer to draw the associated Texture of the Sprite component
 * onto the screen, applying transformations and color modulations as specified by the Sprite.
 */
class SdlSpriteStrategy final : public IRenderingStrategy{
private:
    SDL_Renderer& sdl_renderer_;
public:
    SdlSpriteStrategy(SDL_Renderer& sdl_renderer);
    ~SdlSpriteStrategy() override = default;
    void draw(Component& component) override;
};
