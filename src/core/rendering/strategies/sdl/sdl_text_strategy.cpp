#include <engine/core/rendering/strategies/sdl/sdl_text_strategy.h>

#include <stdexcept>

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/public/components/ui/ui_object.h>
#include <engine/public/components/ui/text.h>

SdlTextStrategy::SdlTextStrategy(SDL_Renderer& sdl_renderer) : sdl_renderer_(sdl_renderer) {}

void SdlTextStrategy::draw(Component& component) {
    auto* text = dynamic_cast<Text*>(&component);
    if (!text) {
        throw std::runtime_error("SdlTextStrategy can only draw Text components.");
    }

    auto parent_opt = component.parent();
    if (!parent_opt.has_value()) {
        throw std::runtime_error("Text has no parent");
    }

    auto& transform = parent_opt->get().transform();

    // If not changed (dirty flag is false), use cached texture
    if (!text->dirty() && texture_) {
        SDL_FRect dst{
            transform.position().x,
            transform.position().y,
            last_font_width_ * transform.scale().x,
            last_font_height_ * transform.scale().y
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

        return;
    }

    auto& font = get_font(text->font(), text->font_path(), text->font_size()).get();

    SDL_Color color{
        static_cast<Uint8>(text->color().r),
        static_cast<Uint8>(text->color().g),
        static_cast<Uint8>(text->color().b),
        static_cast<Uint8>(text->color().a)
    };

    SDL_Surface* surf = TTF_RenderText_Blended(
        &font,
        text->text().c_str(),
        text->text().length(),
        color
    );

    if (!surf) {
        throw std::runtime_error("TTF_RenderText_Blended failed");
    }

    auto w = static_cast<float>(surf->w);;
    auto h = static_cast<float>(surf->h);

    last_font_width_ = w;
    last_font_height_ = h;

    SDL_Texture* new_tex = SDL_CreateTextureFromSurface(&sdl_renderer_, surf);
    SDL_DestroySurface(surf);

    if (!new_tex) {
        throw std::runtime_error("SDL_CreateTextureFromSurface failed");
    }

    texture_.reset(new_tex);
    text->mark_dirty(false);

    SDL_FRect dst{
        transform.position().x,
        transform.position().y,
        last_font_width_ * transform.scale().x,
        last_font_height_ * transform.scale().y
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


std::reference_wrapper<TTF_Font> SdlTextStrategy::get_font(
    const std::string& name,
    const std::string& path, 
    int font_size
) {
    auto& asset_service = Engine::instance()
        .services
        ->get_service<AssetService>()
        .get();

    auto maybe_font = asset_service.try_get_font(name, static_cast<int8_t>(font_size));
    if (maybe_font.has_value()) {
        return maybe_font->get().get_ttf_font();
    }

    auto& font = asset_service.register_font(name, path, static_cast<int8_t>(font_size)).get();
    return font.get_ttf_font();
}