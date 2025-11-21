#pragma once

#include <map>
#include <memory>
#include <string>
#include <SDL3/sdl.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>

/**
 * @brief SDL implementation of the text rendering strategy.
 * 
 * SdlTextStrategy is responsible for rendering text components using SDL.
 * It utilizes an SDL_Renderer to draw the associated text elements of the Component
 * onto the screen, applying necessary transformations and styles as specified by the text component.
 */
class SdlTextStrategy final : public IRenderingStrategy {
private:
    SDL_Renderer& sdl_renderer_;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture_{nullptr, SDL_DestroyTexture};

    float last_font_width_ = 0.0f;
    float last_font_height_ = 0.0f;
    
    std::reference_wrapper<TTF_Font> get_font(const std::string& name, const std::string& path, int size);

public:
    SdlTextStrategy(SDL_Renderer& sdl_renderer);
    ~SdlTextStrategy() override = default;
    void draw(Component& component) override;
};