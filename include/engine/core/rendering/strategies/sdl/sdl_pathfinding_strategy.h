#pragma once

#include <SDL3/SDL.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief SDL implementation of the pathfinding rendering strategy.
 *
 * SdlPathFindingStrategy is responsible for rendering Pathfinding
 * components using SDL. It utilizes an SDL_Renderer to draw the associated
 * Texture of the Pathfinding component onto the screen, applying
 * transformations and color modulations as specified by the Pathfinding.
 */
class SdlPathFindingStrategy final : public IRenderingStrategy {
 private:
  SDL_Renderer& sdl_renderer_;

 public:
  SdlPathFindingStrategy(SDL_Renderer& sdl_renderer);
  ~SdlPathFindingStrategy() override = default;
  void draw(Component& component, Camera& camera) override;
};
