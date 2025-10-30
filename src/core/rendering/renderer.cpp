#include <engine/core/rendering/renderer.h>
#include <engine/public/gameObject.h>
#include <SDL3_image/SDL_image.h>

#include "engine/core/rendering/texture.h"

renderer::renderer(SDL_Renderer* renderer, SDL_Window* window):
        renderer_(SdlRendererPtr{renderer, SDL_DestroyRenderer}),
        window_(SdlWindowPtr{window, SDL_DestroyWindow}) {
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
}
renderer::renderer(SdlRendererPtr renderer, SdlWindowPtr window):
    renderer_(std::move(renderer)),
    window_(std::move(window)){
}

void renderer::clear() const {
    SDL_RenderClear(renderer_.get());
}

void renderer::update() {
    // TODO: Implement
}

// TODO: Remove dummy when implementing Sprite component, and implement the RenderObject method accordingly...
class Sprite : public Component {
    public:
    texture* texture{};
    explicit Sprite(GameObject& parent) : Component(parent) {

    }
};

// TODO: Reimplement this when implementing the renderer
void renderer::render(const std::vector<std::reference_wrapper<GameObject>>& objects) const{
    for (auto gameObjWrapper : objects) {
        auto& gameObj = gameObjWrapper.get();
        const auto& transform = gameObj.transform();
        const auto& absolutePosition = transform.absolute_position();

        for (const auto spriteWrapper : gameObj.get_components<Sprite>()) {
            const Sprite& sprite = spriteWrapper.get();
            const texture& texture = *sprite.texture;
            auto const source = SDL_FRect {
                .x = 0,
                .y = 0,
                .w = texture.width(),
                .h = texture.height()
            };
            auto const target = SDL_FRect {
                .x = absolutePosition.x,
                .y = absolutePosition.y,
                .w = texture.width(),
                .h = texture.height()
            };

            constexpr int scale = 1;
            SDL_RenderTextureTiled(
                renderer_.get(),
                sprite.texture->texture_.get(),
                &source,
                scale,
                &target);
        }
    }
}
