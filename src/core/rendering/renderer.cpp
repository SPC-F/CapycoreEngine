#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <engine/core/engine.h>
#include <engine/core/rendering/renderable.h>
#include <engine/core/rendering/renderer.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/scene_service.h>

constexpr int default_min_aspect_width = 800;
constexpr int default_min_aspect_height = 600;

Renderer::Renderer()
    : Renderer(default_min_aspect_width, default_min_aspect_height, "CapyCore",
               RendererFlags::None) {}

Renderer::Renderer(int min_aspect_width, int min_aspect_height,
                   const std::string& title, RendererFlags flags)
    : sdl_renderer_(nullptr, SDL_DestroyRenderer),
      sdl_window_(nullptr, SDL_DestroyWindow) {
  Uint32 sdl_window_flags = 0;

  if (flags & RendererFlags::Fullscreen) {
    sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
  }
  if (flags & RendererFlags::Borderless) {
    sdl_window_flags |= SDL_WINDOW_BORDERLESS;
  }
  if (flags & RendererFlags::Resizable) {
    sdl_window_flags |= SDL_WINDOW_RESIZABLE;
  }

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) || !TTF_Init()) {
    throw std::runtime_error(
        "Failed to initialize renderer: SDL_Init failed with error: " +
        std::string(SDL_GetError()));
  }

  SDL_Window* window = SDL_CreateWindow(title.c_str(), min_aspect_width,
                                        min_aspect_height, sdl_window_flags);
  if (window == nullptr) {
    throw std::runtime_error(
        "Failed to create window: SDL_CreateWindow failed with error: " +
        std::string(SDL_GetError()));
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    throw std::runtime_error(
        "Failed to create renderer: SDL_CreateRenderer failed with error: " +
        std::string(SDL_GetError()));
  }

  constexpr int default_color = 213;

  SDL_SetRenderDrawColor(renderer, default_color, default_color, default_color,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  sdl_renderer_.reset(renderer);
  sdl_window_.reset(window);

  window_.emplace(min_aspect_width, min_aspect_height);
  window_->init(window);
}

Window& Renderer::window() {
  if (!window_.has_value()) {
    throw std::runtime_error("Window is not initialized in Renderer.");
  }
  return window_.value();
}

void Renderer::clear() const {
    SDL_RenderClear(sdl_renderer_.get());
}

void Renderer::render(const std::map<int, std::vector<std::reference_wrapper<Renderable>>>& objects, Scene& scene) {
    if (objects.empty()) {
        return;
    }

    const auto camera_opt = scene.main_camera();

  if (!camera_opt.has_value() || !camera_opt->get().is_active() ||
      !camera_opt->get().is_main()) {
    return;
  }

    Camera& camera = camera_opt->get();
    const Color bg_color = camera.background_color();

    SDL_RenderClear(sdl_renderer_.get());

    SDL_SetRenderDrawColor(sdl_renderer_.get(), bg_color.r, bg_color.g, bg_color.b, bg_color.a);

    // Since we do not act on the layers, we do not mention them. An alternative here is just accepting the tuple...
    for (auto& renderables_list: objects | std::views::values) {
        for (std::reference_wrapper<Renderable> renderable_wrapper : renderables_list) {
            auto& renderable = renderable_wrapper.get();
            renderable.render_strategy().draw(renderable, camera);
        }
    }

  SDL_RenderPresent(sdl_renderer_.get());
}