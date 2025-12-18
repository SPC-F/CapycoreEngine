#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/strategies/sdl/sdl_box_collider_2d_strategy.h>
#include <engine/physics/creation/physics_creation_flags.h>
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

  auto& box_collider = dynamic_cast<BoxCollider2D&>(component);

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  const auto& camera_position = camera.transform().position();
  float zoom = camera.zoom();

  float half_screen_width = camera.get_screen_width() * 0.5f;
  float half_screen_height = camera.get_screen_height() * 0.5f;

  const auto& box = dynamic_cast<const BoxCollider2D&>(component);

  float cx = body_tf.position.x + box.offset().x + box.width() * 0.5f;
  float cy = body_tf.position.y + box.offset().y + box.height() * 0.5f;

  float w = box.width();
  float h = box.height();

  float rad = rotation * (PhysicsMath::pi / PhysicsMath::circle_divisor);
  float cosA = std::cos(rad);
  float sinA = std::sin(rad);

  SDL_FPoint corners[4] = {
      {-w * 0.5f, -h * 0.5f},
      {w * 0.5f, -h * 0.5f},
      {w * 0.5f, h * 0.5f},
      {-w * 0.5f, h * 0.5f},
  };

  SDL_FPoint screen_corners[4];

  for (int i = 0; i < 4; i++) {
    float rx = corners[i].x * cosA - corners[i].y * sinA;
    float ry = corners[i].x * sinA + corners[i].y * cosA;

    float wx = cx + rx;
    float wy = cy + ry;

    screen_corners[i].x = (wx - camera_position.x) * zoom + half_screen_width;
    screen_corners[i].y = (wy - camera_position.y) * zoom + half_screen_height;
  }

  BodyType2D::Type body_type = box_collider.get_rigidbody().get().type();

  switch (body_type) {
    case BodyType2D::Static:
      SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);
      break;
    case BodyType2D::Kinematic:
      SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 255, 255);
      break;
    case BodyType2D::Dynamic:
      SDL_SetRenderDrawColor(&sdl_renderer_, 0, 255, 0, 255);
      break;
  }

  SDL_RenderLine(&sdl_renderer_, screen_corners[0].x, screen_corners[0].y,
                 screen_corners[1].x, screen_corners[1].y);
  SDL_RenderLine(&sdl_renderer_, screen_corners[1].x, screen_corners[1].y,
                 screen_corners[2].x, screen_corners[2].y);
  SDL_RenderLine(&sdl_renderer_, screen_corners[2].x, screen_corners[2].y,
                 screen_corners[3].x, screen_corners[3].y);
  SDL_RenderLine(&sdl_renderer_, screen_corners[3].x, screen_corners[3].y,
                 screen_corners[0].x, screen_corners[0].y);

  SDL_RenderLine(&sdl_renderer_, screen_corners[0].x, screen_corners[0].y,
                 screen_corners[2].x, screen_corners[2].y);
  SDL_RenderLine(&sdl_renderer_, screen_corners[1].x, screen_corners[1].y,
                 screen_corners[3].x, screen_corners[3].y);

  Color original_color = camera.background_color();
  SDL_SetRenderDrawColor(&sdl_renderer_, original_color.r, original_color.g,
                         original_color.b, original_color.a);
}
