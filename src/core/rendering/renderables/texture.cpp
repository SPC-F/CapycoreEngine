#include <../../../include/engine/core/rendering/renderables/texture.h>
#include "SDL3/SDL_render.h"

Texture::Texture(SDL_Texture* texture)
    : width_{0}, height_{0}, texture_(texture, &SDL_DestroyTexture) {

    SDL_GetTextureSize(texture, &width_, &height_);
}

float Texture::width() const {
    return width_;
}

float Texture::height() const {
    return height_;
}
