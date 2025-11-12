#include <engine/core/rendering/renderer.h>
#include <engine/public/gameObject.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/components/sprite.h>
#include <SDL3_image/SDL_image.h>

Renderer::Renderer() : Renderer(800, 600, "DefaultRenderer", RendererFlags::Borderless) {

}

Renderer::Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags)
    : sdl_renderer_(nullptr, SDL_DestroyRenderer), sdl_window_(nullptr, SDL_DestroyWindow) {

    SDL_WindowFlags sdl_window_flags {SDL_WINDOWPOS_CENTERED};

    if (flags & RendererFlags::Fullscreen) {
        sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags & RendererFlags::Borderless) {
        sdl_window_flags |= SDL_WINDOW_BORDERLESS;
    }
    if(flags & RendererFlags::Resizable) {
        sdl_window_flags |= SDL_WINDOW_RESIZABLE;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(title.c_str(), min_aspect_width, min_aspect_height, sdl_window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "DefaultRenderer");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

void Renderer::render(const std::vector<std::reference_wrapper<GameObject>>& objects) const{
    for (auto gameObjWrapper : objects) {
        auto& gameObj = gameObjWrapper.get();
        const auto& transform = gameObj.transform();
        const auto& position = transform.position();

        for (const auto spriteWrapper : gameObj.get_components<Sprite>()) {
            const Sprite& sprite = spriteWrapper.get();
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

            SDL_RenderTextureRotated(
                    sdl_renderer_.get(),
                    sprite.texture().texture_.get(),
                    &source,
                    &target,
                    transform.rotation(),
                    nullptr, // default is center of target square
                SDL_FLIP_NONE);
        }
    }
}