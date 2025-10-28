#include <engine/core/rendering/Texture.h>
#include "SDL3/SDL_render.h"

Texture::Texture(SDL_Texture* texture)
    : _texture(texture, &SDL_DestroyTexture) {
}

float Texture::width() const {
    return _width;
}

void Texture::width(const float value) {
    _width = value;
}

float Texture::height() const {
    return _height;
}

void Texture::height(const float value) {
    _height = value;
}
