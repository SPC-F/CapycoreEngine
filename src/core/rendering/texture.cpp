#include <engine/core/rendering/texture.h>
#include "SDL3/SDL_render.h"

texture::texture(SDL_Texture* texture)
    : width_{0}, height_{0}, texture_(texture, &SDL_DestroyTexture) {

    SDL_GetTextureSize(texture, &width_, &height_);
}

float texture::width() const {
    return width_;
}

float texture::height() const {
    return height_;
}
