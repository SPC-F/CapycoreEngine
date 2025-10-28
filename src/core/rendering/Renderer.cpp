#include <engine/core/rendering/Renderer.h>
#include <engine/public/gameObject.h>
#include <SDL3_image/SDL_image.h>

#include "engine/core/rendering/Texture.h"

Renderer::Renderer(SDL_Renderer* renderer)
    : _renderer(std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>{renderer, SDL_DestroyRenderer}) {
    SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
}
Renderer::Renderer(SdlRendererPtr renderer)
    : _renderer(std::move(renderer)) {
}

void Renderer::clear() const {
    SDL_RenderClear(_renderer.get());
}
void Renderer::render() const {
    SDL_RenderPresent(_renderer.get());
}

// TODO: Remove dummy when implementing Sprite component, and implement the RenderObject method accordingly...
class Sprite : public Component {
    public:
    Texture* texture;
    explicit Sprite(GameObject& parent) : Component(parent) {

    }
};

// TODO: Reimplement this when implementing the renderer
void Renderer::renderObject(GameObject& gameObj) const{
    const auto& transform = gameObj.transform();
    const auto& absolutePosition = transform.absolute_position();

    for (auto spriteWrapper : gameObj.get_components<Sprite>()) {
        const Sprite& sprite = spriteWrapper.get();
        const Texture& texture = *sprite.texture;
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

        SDL_RenderTextureTiled(
            _renderer.get(),
            sprite.texture->_texture.get(),
            &source,
            1,
            &target);
    }
}
