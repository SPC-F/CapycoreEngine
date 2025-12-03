#include <engine/core/engine.h>
#include <engine/core/rendering/strategies/sdl/sdl_circle_collider_2d_strategy.h>
#include <engine/physics/physics_service.h>
#include <engine/physics/world/body/body_2d.h>
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

  auto& physics_service =
      Engine::instance().services->get_service<PhysicsService>().get();
  if (!physics_service.debug_mode()) {
    return;
  }

  const auto& transform = parent_opt->get().transform();
  const auto& camera_position = camera.transform().position();
  const auto& circle_collider =
      dynamic_cast<const CircleCollider2D&>(component);

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  float radius_px = circle_collider.radius() * transform.scale().x;

  float ox = circle_collider.offset().x;
  float oy = circle_collider.offset().y;

  float angle = transform.rotation();
  float rad = angle * (3.14159265f / 180.0f);
  float cosA = std::cos(rad);
  float sinA = std::sin(rad);

  float rx = ox * cosA - oy * sinA;
  float ry = ox * sinA + oy * cosA;

  Point center{body_tf.position.x + rx - camera_position.x,
               body_tf.position.y + ry - camera_position.y};

  SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);
  draw_circle((int)center.x, (int)center.y, (int)radius_px);

  int dir_x = center.x + std::cos(rad) * radius_px;
  int dir_y = center.y + std::sin(rad) * radius_px;
  SDL_RenderLine(&sdl_renderer_, center.x, center.y, dir_x, dir_y);

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
