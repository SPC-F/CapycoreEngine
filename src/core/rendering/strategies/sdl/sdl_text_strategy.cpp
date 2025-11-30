#include <engine/core/rendering/strategies/sdl/sdl_text_strategy.h>

#include <stdexcept>

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>

constexpr float default_scale_multiplier = 0.5f;
constexpr float default_transform_divider = 0.5f;

SdlTextStrategy::SdlTextStrategy(SDL_Renderer& sdl_renderer) : sdl_renderer_(sdl_renderer) {}

void SdlTextStrategy::draw(Component& component, Camera& camera) {
    auto& text = dynamic_cast<Text&>(component);

    auto parent_opt = component.parent();
    if (!parent_opt.has_value()) {
        throw std::runtime_error("Text has no parent");
    }

    const auto& ui_object = dynamic_cast<const UIObject&>(parent_opt->get());
    const auto& transform = parent_opt->get().transform();

    float scale_x = transform.scale().x;
    float scale_y = transform.scale().y;

    if (!text.dirty() && texture_) {
        
        draw_cached(text, ui_object, transform, scale_x, scale_y);
        return;
    }

    draw_fresh(text, ui_object, transform, scale_x, scale_y);
}

std::reference_wrapper<TTF_Font> SdlTextStrategy::get_font(
    const std::string& name,
    const std::string& path, 
    int font_size
) {
    auto& asset_service = Engine::instance()
        .services
        ->get_service<AssetService>()
        .get();

    auto maybe_font = asset_service.try_get_font(name, font_size);
    if (maybe_font.has_value()) {
        return maybe_font->get().get_ttf_font();
    }

    auto& font = asset_service.register_font(name, path, font_size).get();
    return font.get_ttf_font();
}

void SdlTextStrategy::draw_cached(Text& text, const UIObject& ui_object, const Transform& transform, float scale_x, float scale_y) {
    float x = transform.position().x;
    switch (text.alignment()) {
        case TextAlignment::Left:
            x += 0.0f;
            break;
        case TextAlignment::Center:
            x += (ui_object.width() - last_font_width_) * default_transform_divider;
            break;
        case TextAlignment::Right:
            x += (ui_object.width() - last_font_width_);
            break;
    }

    float y = transform.position().y + ((ui_object.height() - last_font_height_) * default_transform_divider);

    x += text.offset().x;
    y += text.offset().y;

    SDL_FRect dst{
        x * scale_x,
        y * scale_y,
        last_font_width_ * scale_x,
        last_font_height_ * scale_y
    };

    SDL_RenderTextureRotated(
        &sdl_renderer_,
        texture_.get(),
        nullptr,
        &dst,
        transform.rotation(),
        nullptr,
        SDL_FLIP_NONE
    );
}

void SdlTextStrategy::draw_fresh(Text& text, const UIObject& ui_object, const Transform& transform, float scale_x, float scale_y) {
    auto& font = SdlTextStrategy::get_font(text.font(), text.font_path(), text.font_size()).get();
    SDL_Color color{
        static_cast<Uint8>(text.color().r),
        static_cast<Uint8>(text.color().g),
        static_cast<Uint8>(text.color().b),
        static_cast<Uint8>(text.color().a)
    };

    if (text.text().empty()) {
        last_font_width_ = 0.0f;
        last_font_height_ = 0.0f;
        text.mark_dirty(false);
        texture_.reset();
        return;
    }

    std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> surf(
        TTF_RenderText_Blended(&font, text.text().c_str(), text.text().length(), color),
        SDL_DestroySurface
    );

    if (!surf) throw std::runtime_error("TTF_RenderText_Blended failed");

    last_font_width_ = static_cast<float>(surf->w);
    last_font_height_ = static_cast<float>(surf->h);

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> new_tex(
        SDL_CreateTextureFromSurface(&sdl_renderer_, surf.get()),
        SDL_DestroyTexture
    );
    surf.reset();

    if (!new_tex) throw std::runtime_error("SDL_CreateTextureFromSurface failed");

    texture_.reset(new_tex.release());
    text.mark_dirty(false);

    float x = transform.position().x;
    switch (text.alignment()) {
        case TextAlignment::Left:   
            x += 0.0f; 
            break;
        case TextAlignment::Center: 
            x += (ui_object.width() - last_font_width_) * default_transform_divider; 
            break;
        case TextAlignment::Right:  
            x += (ui_object.width() - last_font_width_); 
            break;
    }

    float y = transform.position().y + ((ui_object.height() - last_font_height_) * default_transform_divider);

    x += text.offset().x;
    y += text.offset().y;

    SDL_FRect dst{
        x * scale_x,
        y * scale_y,
        last_font_width_ * scale_x,
        last_font_height_ * scale_y
    };

    SDL_SetTextureScaleMode(texture_.get(), SDL_SCALEMODE_NEAREST);
    SDL_RenderTextureRotated(
        &sdl_renderer_,
        texture_.get(),
        nullptr,
        &dst,
        transform.rotation(),
        nullptr,
        SDL_FLIP_NONE
    );
}