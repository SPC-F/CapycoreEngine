#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/strategies/sdl/sdl_box_collider_2d_strategy.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/physics_service.h>
#include <engine/public/components/colliders/box_collider_2d.h>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlBoxCollider2DStrategy::SdlBoxCollider2DStrategy(SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

void SdlBoxCollider2DStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "Cannot draw BoxCollider2D component without a parent GameObject");
  }

  auto& physics_service =
      Engine::instance().services->get_service<PhysicsService>().get();
  if (!physics_service.debug_mode()) {
    return;
  }

  const auto& transform = parent_opt->get().transform();
  const auto& position = transform.position();
  const auto& rotation = transform.rotation();

  const auto& camera_position = camera.transform().position();
  const auto& box_collider = dynamic_cast<const BoxCollider2D&>(component);

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  float world_x = body_tf.position.x + box_collider.offset().x;
  float world_y = body_tf.position.y + box_collider.offset().y;

  float w = box_collider.width();
  float h = box_collider.height();

  float cx = world_x + w * 0.5f;
  float cy = world_y + h * 0.5f;

  float angleDegrees = rotation;
  float rad = angleDegrees * (PhysicsMath::pi / PhysicsMath::circle_divisor);
  float cosA = std::cos(rad);
  float sinA = std::sin(rad);

  SDL_FPoint corners[4] = {
      {-w * 0.5f, -h * 0.5f},
      {w * 0.5f, -h * 0.5f},
      {w * 0.5f, h * 0.5f},
      {-w * 0.5f, h * 0.5f},
  };

  for (auto& p : corners) {
    float rx = p.x * cosA - p.y * sinA;
    float ry = p.x * sinA + p.y * cosA;

    p.x = rx + cx - camera_position.x;
    p.y = ry + cy - camera_position.y;
  }

  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 255, 0, 255);

  SDL_RenderLine(&sdl_renderer_, corners[0].x, corners[0].y, corners[1].x,
                 corners[1].y);
  SDL_RenderLine(&sdl_renderer_, corners[1].x, corners[1].y, corners[2].x,
                 corners[2].y);
  SDL_RenderLine(&sdl_renderer_, corners[2].x, corners[2].y, corners[3].x,
                 corners[3].y);
  SDL_RenderLine(&sdl_renderer_, corners[3].x, corners[3].y, corners[0].x,
                 corners[0].y);

  SDL_RenderLine(&sdl_renderer_, corners[0].x, corners[0].y, corners[2].x,
                 corners[2].y);
  SDL_RenderLine(&sdl_renderer_, corners[1].x, corners[1].y, corners[3].x,
                 corners[3].y);

  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 0, 255);
}
