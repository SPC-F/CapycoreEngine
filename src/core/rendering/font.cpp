#include <engine/core/rendering/font.h>

#include <SDL3_ttf/SDL_ttf.h>

Font::Font(std::string name, int8_t size, TTF_Font* font)
    : name_{std::move(name)}, size_{size}, font_{font, &TTF_CloseFont} {
}

std::string Font::get_name() const {
    return name_;
}

int8_t Font::get_size() const {
    return size_;
}

TTF_Font& Font::get_ttf_font() const {
    return *font_;
}