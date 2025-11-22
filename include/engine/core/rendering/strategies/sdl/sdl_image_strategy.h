#pragma once

#include <SDL3/SDL.h>

#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief SDL implementation of the image rendering strategy.
 * 
 * SdlImageStrategy is responsible for rendering Image components using SDL.
 * It utilizes an SDL_Renderer to draw the associated Texture of the Image component
 * onto the screen, applying transformations and color modulations as specified by the Image.
 */
class SdlImageStrategy final : public IRenderingStrategy{
private:
    SDL_Renderer& sdl_renderer_;

    Color get_default_image_color(SDL_Texture* texture);
    void set_image_color(const Color& color, SDL_Texture* texture);
public:
    SdlImageStrategy(SDL_Renderer& sdl_renderer);
    ~SdlImageStrategy() override = default;
    void draw(Component& component) override;
};
