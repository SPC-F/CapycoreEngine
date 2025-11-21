#include <engine/core/rendering/strategies/sdl/sdl_image_strategy.h>

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/public/components/ui/image.h>
#include <engine/public/components/ui/ui_object.h>

constexpr float default_texture_width = 32;
constexpr float default_texture_height = 32;

SdlImageStrategy::SdlImageStrategy(SDL_Renderer& sdl_renderer) : sdl_renderer_(sdl_renderer) {}

Color SdlImageStrategy::get_default_image_color(SDL_Texture* texture) {
    Color color;
    SDL_GetTextureColorMod(texture,
                           reinterpret_cast<Uint8*>(&color.r),
                           reinterpret_cast<Uint8*>(&color.g),
                           reinterpret_cast<Uint8*>(&color.b));
    SDL_GetTextureAlphaMod(texture, reinterpret_cast<Uint8*>(&color.a));
    return color;
}

void SdlImageStrategy::set_image_color(const Color& color, SDL_Texture* texture) {
    SDL_SetTextureColorMod(texture,
                           static_cast<Uint8>(color.r),
                           static_cast<Uint8>(color.g),
                           static_cast<Uint8>(color.b));
    SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(color.a));
}

void SdlImageStrategy::draw(Component& component) {
    auto parent_opt = component.parent();
    if (!parent_opt.has_value()) {
        throw std::runtime_error("Cannot draw Image component without a parent GameObject");
    }

    const auto& transform = parent_opt->get().transform();
    const auto& position = transform.position();

    const auto& ui_object = dynamic_cast<const UIObject&>(parent_opt->get());
    const auto& image = dynamic_cast<const Image&>(component);
    const Texture& texture = image.texture();
    auto* texture_ptr = texture.texture_.get();

    float width = ui_object.width();
    float height = ui_object.height();

    SDL_FRect target {
        .x = position.x,
        .y = position.y,
        .w = width * transform.scale().x,
        .h = height * transform.scale().y
    };

    Color original_color = get_default_image_color(texture_ptr);
    set_image_color(image.color(), texture_ptr);

    SDL_SetTextureScaleMode(texture_ptr, SDL_SCALEMODE_NEAREST);

    SDL_RenderTextureRotated(
        &sdl_renderer_,
        texture_ptr,
        nullptr,
        &target,
        transform.rotation(),
        nullptr,                // pivot is already accountend for in the position
        SDL_FLIP_NONE
    );

    set_image_color(original_color, texture_ptr);
}

