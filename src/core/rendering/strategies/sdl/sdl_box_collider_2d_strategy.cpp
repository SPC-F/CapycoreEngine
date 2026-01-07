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
  if (!parent_opt.has_value()) return;

  auto& box_collider = dynamic_cast<BoxCollider2D&>(component);

  /// Get the Rigidbody2D to access the physics body
  auto& rb = parent_opt->get().get_component<Rigidbody2D>().value().get();
  auto body = rb.body();
  b2ShapeId shape_id = body.shapes[0].id;

  /// We directly request the box corners from the Body2D utility
  /// which computes them based on the body's transform and shape.
  ///
  /// The reason we do not use the GameObject's transform is that
  /// the Rigidbody2D position/rotation might differ due to
  /// shape size, offset, and physics simulation.
  PolygonVerts corners = Body2D::get_body_polygon_verts(body, shape_id);

  /// Set up screen coordinates
  SDL_FPoint screenVerts[4];
  auto cam = camera.transform().position();
  float zoom = camera.zoom();
  float half_screen_w = camera.get_screen_width() * 0.5f;
  float half_screen_h = camera.get_screen_height() * 0.5f;

  /// Transform physics world coordinates to screen coordinates
  for (int i = 0; i < 4; i++) {
    screenVerts[i].x = (corners.verts[i].x - cam.x) * zoom + half_screen_w;
    screenVerts[i].y = (corners.verts[i].y - cam.y) * zoom + half_screen_h;
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

  for (int i = 0; i < 4; i++) {
    int j = (i + 1) % 4;
    SDL_RenderLine(&sdl_renderer_, screenVerts[i].x, screenVerts[i].y,
                   screenVerts[j].x, screenVerts[j].y);
  }

  Color original_color = camera.background_color();
  SDL_SetRenderDrawColor(&sdl_renderer_, original_color.r, original_color.g,
                         original_color.b, original_color.a);
}