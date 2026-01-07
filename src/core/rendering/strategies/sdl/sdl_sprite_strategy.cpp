#include "engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h"

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/physics/physics_math.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/components/sprite.h>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlSpriteStrategy::SdlSpriteStrategy(SDL_Renderer& sdl_renderer)
    : sdl_renderer_(sdl_renderer) {}

Color SdlSpriteStrategy::get_default_sprite_color(
    SDL_Texture*
        texture) {  // NOLINT [readability-convert-member-functions-to-static]
  Color color;
  SDL_GetTextureColorMod(texture, reinterpret_cast<Uint8*>(&color.r),
                         reinterpret_cast<Uint8*>(&color.g),
                         reinterpret_cast<Uint8*>(&color.b));
  SDL_GetTextureAlphaMod(texture, reinterpret_cast<Uint8*>(&color.a));
  return color;
}

void SdlSpriteStrategy::set_sprite_color(
    const Color& color,
    SDL_Texture*
        texture) {  // NOLINT [readability-convert-member-functions-to-static]
  SDL_SetTextureColorMod(texture, static_cast<Uint8>(color.r),
                         static_cast<Uint8>(color.g),
                         static_cast<Uint8>(color.b));
  SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(color.a));
}

void SdlSpriteStrategy::draw(Component& component, Camera& camera) {
  auto parent_opt = component.parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "Cannot draw Sprite component without a parent GameObject");
  }

  auto& parent = parent_opt->get();
  const auto& transform = parent.transform();

  /// We check if there's a Rigidbody2D component to get physics-based
  /// position/rotation
  std::optional<std::reference_wrapper<Rigidbody2D>> rb_opt;
  if (parent.get_component<Rigidbody2D>().has_value()) {
    rb_opt = parent.get_component<Rigidbody2D>().value();
  }

  /// Fetch the Sprite component
  const auto& sprite = dynamic_cast<const Sprite&>(component);
  const Texture& texture = sprite.texture();
  auto* texture_ptr = texture.texture_.get();

  float width = default_texture_width;
  float height = default_texture_height;

  if (texture_ptr != nullptr) {
    width = static_cast<float>(texture_ptr->w);
    height = static_cast<float>(texture_ptr->h);
  }

  /// Compute camera and zoom related values
  auto const camera_position = camera.transform().position();
  const float zoom = camera.zoom();
  const float half_screen_width = camera.get_screen_width() * 0.5f;
  const float half_screen_height = camera.get_screen_height() * 0.5f;

  /// Set up target rectangle for rendering
  SDL_FRect target{};
  float rotation = transform.rotation();
  float scale_x = transform.scale().x;
  float scale_y = transform.scale().y;
  float pos_x = transform.position().x;
  float pos_y = transform.position().y;

  /// If Rigidbody2D exists, override position and rotation DIRECTLY from
  /// physics body See sdl_box_collider_2d_strategy.cpp for explanation why we
  /// cannot use GameObject transform here
  if (rb_opt.has_value()) {
    auto& rb = rb_opt->get();
    auto body = rb.body();

    Vec2f body_pos = Body2D::get_body_world_position(body);
    pos_x = body_pos.x;
    pos_y = body_pos.y;
    rotation = Body2D::get_body_world_rotation(body);
  }

  /// Apply camera transformations
  target.x = (pos_x - camera_position.x) * zoom + half_screen_width -
             0.5f * width * scale_x * zoom;
  target.y = (pos_y - camera_position.y) * zoom + half_screen_height -
             0.5f * height * scale_y * zoom;
  target.w = width * scale_x * zoom;
  target.h = height * scale_y * zoom;

  const Color original_color = get_default_sprite_color(texture_ptr);
  set_sprite_color(sprite.color(), texture_ptr);

  SDL_FlipMode flip_mode = SDL_FLIP_NONE;
  if (sprite.flip_x() && sprite.flip_y())
    flip_mode =
        static_cast<SDL_FlipMode>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
  else if (sprite.flip_x())
    flip_mode = SDL_FLIP_HORIZONTAL;
  else if (sprite.flip_y())
    flip_mode = SDL_FLIP_VERTICAL;

  SDL_RenderTextureRotated(&sdl_renderer_, texture_ptr, nullptr, &target,
                           rotation,
                           nullptr,  /// pivot => center by default
                           flip_mode);

  set_sprite_color(original_color, texture_ptr);
}
