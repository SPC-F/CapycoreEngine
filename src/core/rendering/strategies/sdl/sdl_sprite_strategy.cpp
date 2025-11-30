#include "engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h"

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/public/components/sprite.h>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlSpriteStrategy::SdlSpriteStrategy(SDL_Renderer& sdl_renderer) : sdl_renderer_(sdl_renderer) {}

Color SdlSpriteStrategy::get_default_sprite_color(SDL_Texture* texture) { // NOLINT [readability-convert-member-functions-to-static]
    Color color;
    SDL_GetTextureColorMod(texture,
                           reinterpret_cast<Uint8*>(&color.r),
                           reinterpret_cast<Uint8*>(&color.g),
                           reinterpret_cast<Uint8*>(&color.b));
    SDL_GetTextureAlphaMod(texture, reinterpret_cast<Uint8*>(&color.a));
    return color;
}

void SdlSpriteStrategy::set_sprite_color(const Color& color, SDL_Texture* texture) { // NOLINT [readability-convert-member-functions-to-static]
    SDL_SetTextureColorMod(texture,
                           static_cast<Uint8>(color.r),
                           static_cast<Uint8>(color.g),
                           static_cast<Uint8>(color.b));
    SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(color.a));
}

void SdlSpriteStrategy::draw(Component& component, Camera& camera) {
    auto parent_opt = component.parent();
    if (!parent_opt.has_value()) {
        throw std::runtime_error("Cannot draw Sprite component without a parent GameObject");
    }

    const auto& transform = parent_opt->get().transform();
    const auto& position = transform.position();
    const auto& camera_position = camera.transform().position();

    const auto& sprite = dynamic_cast<const Sprite&>(component);
    const Texture& texture = sprite.texture();
    auto* texture_ptr = texture.texture_.get();

    float width = default_texture_width;
    float height = default_texture_height;

    auto const source = SDL_FRect {
        .x = 0,
        .y = 0,
        .w = width,
        .h = height
    };

    auto const target = SDL_FRect {
        .x = position.x - camera_position.x,
        .y = position.y - camera_position.y,
        .w = width * transform.scale().x,
        .h = height * transform.scale().y
    };

    const Color original_color = get_default_sprite_color(texture_ptr);
    set_sprite_color(sprite.color(), texture_ptr);

    SDL_RenderTextureRotated(
        &sdl_renderer_,
        texture_ptr,
        &source,
        &target,
        transform.rotation(),
        nullptr, // pivot = center
        SDL_FLIP_NONE
    );

    set_sprite_color(original_color, texture_ptr);
}
