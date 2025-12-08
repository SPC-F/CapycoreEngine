#pragma once

#include <SDL3/SDL.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief SDL implementation of the navigation node rendering strategy.
 *
 * SdlNavigationNodeStrategy is responsible for rendering NavigationNode
 * components using SDL. It utilizes an SDL_Renderer to draw the associated
 * Texture of the NavigationNode component onto the screen, applying
 * transformations and color modulations as specified by the NavigationNode.
 */
class SdlNavigationNodeStrategy final : public IRenderingStrategy {
 private:
  SDL_Renderer& sdl_renderer_;

 public:
  SdlNavigationNodeStrategy(SDL_Renderer& sdl_renderer);
  ~SdlNavigationNodeStrategy() override = default;
  void draw(Component& component, Camera& camera) override;
};
