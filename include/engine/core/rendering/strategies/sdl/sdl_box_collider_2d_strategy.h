#pragma once

#include <SDL3/SDL.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief SDL implementation of the box collider 2D rendering strategy.
 *
 * SdlBoxCollider2DStrategy is responsible for rendering BoxCollider2D
 * components using SDL. It utilizes an SDL_Renderer to draw the associated
 * Texture of the BoxCollider2D component onto the screen, applying
 * transformations and color modulations as specified by the BoxCollider2D.
 */
class SdlBoxCollider2DStrategy final : public IRenderingStrategy {
 private:
  SDL_Renderer& sdl_renderer_;

 public:
  SdlBoxCollider2DStrategy(SDL_Renderer& sdl_renderer);
  ~SdlBoxCollider2DStrategy() override = default;
  void draw(Component& component, Camera& camera) override;
};
