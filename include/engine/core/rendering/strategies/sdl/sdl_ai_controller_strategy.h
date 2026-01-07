#pragma once

#include <SDL3/SDL.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief SDL implementation of the AIController rendering strategy.
 *
 * SdlAIControllerStrategy is responsible for rendering AIController
 * components using SDL. It utilizes an SDL_Renderer to draw the associated
 * Texture of the AIController component onto the screen, applying
 * transformations and color modulations as specified by the AIController.
 */
class SdlAIControllerStrategy final : public IRenderingStrategy {
 private:
  SDL_Renderer& sdl_renderer_;

 public:
  SdlAIControllerStrategy(SDL_Renderer& sdl_renderer);
  ~SdlAIControllerStrategy() override = default;
  void draw(Component& component, Camera& camera) override;
};
