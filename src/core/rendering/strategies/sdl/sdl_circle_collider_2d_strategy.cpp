#include <engine/core/engine.h>
#include <engine/core/rendering/strategies/sdl/sdl_circle_collider_2d_strategy.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/physics_service.h>
#include <engine/physics/world/body/body_2d.h>
#include <engine/public/components/colliders/circle_collider_2d.h>

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
  float zoom = camera.zoom();

  float half_screen_width = camera.get_screen_width() * 0.5f;
  float half_screen_height = camera.get_screen_height() * 0.5f;

  const auto& circle_collider =
      dynamic_cast<const CircleCollider2D&>(component);

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  float radius_px = circle_collider.radius() * transform.scale().x;

  float ox = circle_collider.offset().x;
  float oy = circle_collider.offset().y;

  float angle = transform.rotation();
  float rad = angle * (PhysicsMath::pi / PhysicsMath::circle_divisor);
  float cosA = std::cos(rad);
  float sinA = std::sin(rad);

  float rx = ox * cosA - oy * sinA;
  float ry = ox * sinA + oy * cosA;

  float world_cx = body_tf.position.x + rx;
  float world_cy = body_tf.position.y + ry;

  Point center{(world_cx - camera_position.x) * zoom + half_screen_width,
               (world_cy - camera_position.y) * zoom + half_screen_height};

  float screen_radius = radius_px * zoom;

  SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);

  draw_circle((int)center.x, (int)center.y, (int)screen_radius);

  auto rotate_local = [&](float lx, float ly) -> SDL_FPoint {
    return {lx * cosA - ly * sinA, lx * sinA + ly * cosA};
  };

  SDL_FPoint horizL = rotate_local(-radius_px, 0);
  SDL_FPoint horizR = rotate_local(radius_px, 0);
  SDL_FPoint vertT = rotate_local(0, -radius_px);
  SDL_FPoint vertB = rotate_local(0, radius_px);

  auto to_screen = [&](SDL_FPoint p) -> SDL_FPoint {
    return {p.x + center.x, p.y + center.y};
  };

  SDL_FPoint H1 = to_screen(horizL);
  SDL_FPoint H2 = to_screen(horizR);
  SDL_FPoint V1 = to_screen(vertT);
  SDL_FPoint V2 = to_screen(vertB);

  SDL_RenderLine(&sdl_renderer_, H1.x, H1.y, H2.x, H2.y);
  SDL_RenderLine(&sdl_renderer_, V1.x, V1.y, V2.x, V2.y);

  Color original_color = camera.background_color();
  SDL_SetRenderDrawColor(&sdl_renderer_, original_color.r, original_color.g,
                         original_color.b, original_color.a);
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
