#include <engine/public/components/sprite.h>
#include <engine/core/assetManager.h>
#include <engine/core/engine.h>
#include <format>

Texture& get_texture_for(const std::string& sprite) {
    const auto& service = Engine::instance().services->getService<AssetManager>().get();
    const auto maybe_texture = service.try_get_texture(sprite);
    if (!maybe_texture.has_value()) {
        throw std::runtime_error(std::format("Failed to get texture for sprite: {}", sprite));
    }
    auto const resource = maybe_texture.value();
    return resource.get();
}

Sprite::Sprite(GameObject& parent, const std::string& sprite, const Color color, const int flipX, const int flipY, const int sortingLayer, const int orderingLayer)
    : Component(parent),
    texture_(get_texture_for(sprite)),
    flipX_(flipX),
    flipY_(flipY),
    sortingLayer_(sortingLayer),
    orderingLayer_(orderingLayer),
    color_(color) {
}

int Sprite::flip_x() const {
    return flipX_;
}
Sprite& Sprite::flip_x(const int val) {
    flipX_ = val;
    return *this;
}

int Sprite::flip_y() const {
    return flipY_;
}
Sprite& Sprite::flip_y(const int val) {
    flipY_ = val;
    return *this;
}

int Sprite::sortingLayer() const {
    return sortingLayer_;
}
Sprite& Sprite::sortingLayer(const int val) {
    sortingLayer_ = val;
    return *this;
}

int Sprite::orderingLayer() const {
    return orderingLayer_;
}
Sprite& Sprite::orderingLayer(const int val) {
    orderingLayer_ = val;
    return *this;
}

Color Sprite::color() const {
    return color_;
}

Sprite& Sprite::color(const Color color) {
    this->color_ = color;
    return *this;
}

const Texture& Sprite::texture() const {
    return texture_;
}