#include <engine/core/rendering/renderer.h>
#include <engine/public/gameObject.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/components/sprite.h>
#include <SDL3_image/SDL_image.h>

constexpr int default_min_aspect_width = 800;
constexpr int default_min_aspect_height = 600;
Renderer::Renderer() : Renderer(default_min_aspect_width, default_min_aspect_height, "CapyCore", RendererFlags::None) {

}

Renderer::Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags)
    : sdl_renderer_(nullptr, SDL_DestroyRenderer), sdl_window_(nullptr, SDL_DestroyWindow) {

    Uint32 sdl_window_flags = 0;

    if (flags & RendererFlags::Fullscreen)
        sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
    if (flags & RendererFlags::Borderless)
        sdl_window_flags |= SDL_WINDOW_BORDERLESS;
    if (flags & RendererFlags::Resizable)
        sdl_window_flags |= SDL_WINDOW_RESIZABLE;

    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        throw std::runtime_error("Failed to initialize renderer: SDL_Init failed with error: " + std::string(SDL_GetError()));
    }

    SDL_Window* window = SDL_CreateWindow(title.c_str(), min_aspect_width, min_aspect_height, sdl_window_flags);
    if(window == nullptr) {
        throw std::runtime_error("Failed to create window: SDL_CreateWindow failed with error: " + std::string(SDL_GetError()));
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if(renderer == nullptr) {
        throw std::runtime_error("Failed to create renderer: SDL_CreateRenderer failed with error: " + std::string(SDL_GetError()));
    }

    constexpr int default_color = 213;

    SDL_SetRenderDrawColor(renderer, default_color, default_color, default_color, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    sdl_renderer_.reset(renderer);
    sdl_window_.reset(window);

    window_.emplace(min_aspect_width, min_aspect_height);
    window_->init(window);
}

Window& Renderer::window() {
    return window_.value();
}

void Renderer::clear() const {
    SDL_RenderClear(sdl_renderer_.get());
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

void Renderer::render(const std::vector<std::reference_wrapper<GameObject>>& objects) const{
    SDL_RenderClear(sdl_renderer_.get());

    for (auto game_obj_wrapper : objects) {
        auto& game_obj = game_obj_wrapper.get();
        const auto& transform = game_obj.transform();
        const auto& position = transform.position();

        for (const auto sprite_wrapper : game_obj.get_components<Sprite>()) {
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
                    sdl_renderer_.get(),
                    sprite.texture().texture_.get(),
                    &source,
                    &target,
                    transform.rotation(),
                    nullptr, // default is center of target square
                SDL_FLIP_NONE);

            set_color(original_color, texture.texture_.get());
        }
    }

    SDL_RenderPresent(sdl_renderer_.get());
}