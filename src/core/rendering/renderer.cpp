#include <SDL3/sdl.h>
#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/texture.h>
#include <engine/core/rendering/renderable.h>

constexpr int default_min_aspect_width = 800;
constexpr int default_min_aspect_height = 600;
Renderer::Renderer() : Renderer(default_min_aspect_width, default_min_aspect_height, "CapyCore", RendererFlags::None) {}

Renderer::Renderer(int min_aspect_width, int min_aspect_height, const std::string& title, RendererFlags flags)
    : sdl_renderer_(nullptr, SDL_DestroyRenderer), sdl_window_(nullptr, SDL_DestroyWindow) {

    Uint32 sdl_window_flags = 0;

    if (flags & RendererFlags::Fullscreen) {
        sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags & RendererFlags::Borderless) {
        sdl_window_flags |= SDL_WINDOW_BORDERLESS;
    }
    if (flags & RendererFlags::Resizable){
        sdl_window_flags |= SDL_WINDOW_RESIZABLE;
    }

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
    if(!window_.has_value()) {
        throw std::runtime_error("Window is not initialized in Renderer.");
    }
    return window_.value();
}

void Renderer::clear() const {
    SDL_RenderClear(sdl_renderer_.get());
}

void Renderer::render(std::vector<std::reference_wrapper<GameObject>>& objects) {
    SDL_RenderClear(sdl_renderer_.get());

    for (auto game_obj_wrapper : objects) {
        auto& game_obj = game_obj_wrapper.get();
        auto renderables = game_obj.get_components<Renderable>();
        
        for (auto renderable_wrapper : renderables) {
            auto& renderable = renderable_wrapper.get();
            auto& strategy = renderable.render_strategy();

            strategy.draw(renderable);
        }
    }

    SDL_RenderPresent(sdl_renderer_.get());
}