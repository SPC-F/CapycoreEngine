#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/core/rendering/strategies/sdl/sdl_box_collider_2d_strategy.h>
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

  const auto& transform = parent_opt->get().transform();
  const auto& position = transform.position();
  const auto& camera_position = camera.transform().position();
  const auto& box_collider = dynamic_cast<const BoxCollider2D&>(component);

  const auto& body_tf = Body2D::get_pixel_transform(
      parent_opt->get().get_component<Rigidbody2D>().value().get().body());

  SDL_FRect box{
      .x = body_tf.position.x - camera_position.x + box_collider.offset().x,
      .y = body_tf.position.y - camera_position.y + box_collider.offset().y,
      .w = box_collider.width(),
      .h = box_collider.height(),
  };

  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 255, 0, 255);  // Green
  SDL_RenderRect(&sdl_renderer_, &box);  // note SDL_RenderRectF for SDL_FRect
  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 0, 255);    // Reset to black
  SDL_SetRenderDrawColor(&sdl_renderer_, 255, 0, 0, 255);  // Red for X
  SDL_RenderLine(&sdl_renderer_, box.x, box.y, box.x + box.w, box.y + box.h);
  SDL_RenderLine(&sdl_renderer_, box.x + box.w, box.y, box.x, box.y + box.h);
  SDL_SetRenderDrawColor(&sdl_renderer_, 0, 0, 0, 255);  // Reset to black

  // const auto& sprite = dynamic_cast<const Sprite&>(component);
  // const Texture& texture = sprite.texture();
  // auto* texture_ptr = texture.texture_.get();

  // float width = default_texture_width;
  // float height = default_texture_height;

  // if (texture_ptr != nullptr) {
  //   width = static_cast<float>(texture_ptr->w);
  //   height = static_cast<float>(texture_ptr->h);
  // }

  // auto const source = SDL_FRect{.x = 0, .y = 0, .w = width, .h = height};

  // auto const target = SDL_FRect{.x = position.x - camera_position.x,
  //                               .y = position.y - camera_position.y,
  //                               .w = width * transform.scale().x,
  //                               .h = height * transform.scale().y};

  // const Color original_color = get_default_sprite_color(texture_ptr);
  // set_sprite_color(sprite.color(), texture_ptr);

  // SDL_RenderTextureRotated(&sdl_renderer_, texture_ptr, &source, &target,
  //                          transform.rotation(),
  //                          nullptr,  // pivot = center
  //                          SDL_FLIP_NONE);

  // set_sprite_color(original_color, texture_ptr);
}
