#include "engine/core/rendering/strategies/sdl/sdl_sprite_strategy.h"

SdlSpriteStrategy::SdlSpriteStrategy(Sprite& sprite, GameObject& parent, SDL_Renderer* renderer):
    sprite_(sprite),
    game_object_(parent),
    sdl_renderer_(renderer) {

}

Color get_default_color(SDL_Texture* texture) {
    Color color;
    SDL_GetTextureColorMod(texture,
                           reinterpret_cast<Uint8*>(&color.r),
                           reinterpret_cast<Uint8*>(&color.g),
                           reinterpret_cast<Uint8*>(&color.b));
    SDL_GetTextureAlphaMod(texture, reinterpret_cast<Uint8*>(&color.a));
    return color;
}

void set_color(const Color& color, SDL_Texture* texture) {
    SDL_SetTextureColorMod(texture,
                           static_cast<Uint8>(color.r),
                           static_cast<Uint8>(color.g),
                           static_cast<Uint8>(color.b));
    SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(color.a));
}

void SdlSpriteStrategy::draw(Renderer &renderer) {
    const auto& transform = game_object_.transform();
    const auto& position = transform.position();

    for (const auto sprite_wrapper : game_object_.get_components<Sprite>()) {
        const Sprite& sprite = sprite_wrapper.get();
        const Texture& texture = sprite.texture();

        auto const source = SDL_FRect {
                .x = 0,
                .y = 0,
                .w = texture.width(),
                .h = texture.height()
        };
        auto const target = SDL_FRect {
                .x = position.x,
                .y = position.y,
                .w = texture.width() * transform.scale().x,
                .h = texture.height() * transform.scale().y
        };

        Color original_color = get_default_color(texture.texture_.get());
        set_color(sprite.color(), texture.texture_.get());

        SDL_RenderTextureRotated(
                sdl_renderer_,
                sprite.texture().texture_.get(),
                &source,
                &target,
                transform.rotation(),
                nullptr, // default is center of target square
                SDL_FLIP_NONE);

        set_color(original_color, texture.texture_.get());
    }
}
