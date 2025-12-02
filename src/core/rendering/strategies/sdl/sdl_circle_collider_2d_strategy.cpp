#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/strategies/sdl/sdl_circle_collider_2d_strategy.h>
#include <engine/physics/physics_service.h>
#include <engine/physics/world/body/body_2d.h>
#include <engine/physics/world/physics_world.h>
#include <engine/public/components/colliders/circle_collider_2d.h>

#include <iostream>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlCircleCollider2DStrategy::SdlCircleCollider2DStrategy(
    SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

void SdlCircleCollider2DStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "Cannot draw CircleCollider2D component without a parent GameObject");
  }

  const auto& transform = parent_opt->get().transform();
  const auto& position = transform.position();
  const auto& camera_position = camera.transform().position();
  const auto& circle_collider =
      dynamic_cast<const CircleCollider2D&>(component);

  auto& physics =
      Engine::instance().services->get_service<PhysicsService>().get();

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  float radius_px = circle_collider.radius() * transform.scale().x;

  Point center{
      body_tf.position.x - camera_position.x + circle_collider.offset().x,
      body_tf.position.y - camera_position.y + circle_collider.offset().y};

  SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);
  draw_circle((int)center.x, (int)center.y, (int)radius_px);
  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 0, 255);
}

void SdlCircleCollider2DStrategy::draw_circle(int cx, int cy, int radius) {
  int x = radius - 1;
  int y = 0;
  int dx = 1;
  int dy = 1;
  int err = dx - (radius << 1);

  while (x >= y) {
    SDL_RenderPoint(&sdl_renderer_, cx + x, cy + y);
    SDL_RenderPoint(&sdl_renderer_, cx + y, cy + x);
    SDL_RenderPoint(&sdl_renderer_, cx - y, cy + x);
    SDL_RenderPoint(&sdl_renderer_, cx - x, cy + y);
    SDL_RenderPoint(&sdl_renderer_, cx - x, cy - y);
    SDL_RenderPoint(&sdl_renderer_, cx - y, cy - x);
    SDL_RenderPoint(&sdl_renderer_, cx + y, cy - x);
    SDL_RenderPoint(&sdl_renderer_, cx + x, cy - y);

    if (err <= 0) {
      y++;
      err += dy;
      dy += 2;
    }

    if (err > 0) {
      x--;
      dx += 2;
      err += dx - (radius << 1);
    }
  }
}
