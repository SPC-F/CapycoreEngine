#include <engine/core/rendering/renderer.h>
#include <engine/public/gameObject.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/components/sprite.h>
#include <SDL3_image/SDL_image.h>

Renderer::Renderer() : renderer_(nullptr, SDL_DestroyRenderer), window_(nullptr, SDL_DestroyWindow) {
    SDL_Window* window = SDL_CreateWindow("Capycore", 500, 500, SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "DefaultRenderer");
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
    renderer_.reset(renderer);
    window_.reset(window);
}

void Renderer::clear() const {
    SDL_RenderClear(renderer_.get());
}

void Renderer::update() {
    // TODO: Implement
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
                .w = texture.width(),
                .h = texture.height()
            };

            constexpr int scale = 1;
            SDL_RenderTextureTiled(
                renderer_.get(),
                sprite.texture().texture_.get(),
                &source,
                scale,
                &target);
        }
    }
}
