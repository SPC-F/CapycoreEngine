#include <engine/public/components/sprite.h>
#include <engine/core/assetManager.h>
#include <engine/core/engine.h>
#include <format>

Texture& get_texture_for(const std::string& sprite) {
    const auto& service = Engine::instance().services->get_service<AssetManager>().get();
    const auto maybe_texture = service.try_get_texture(sprite);
    if (!maybe_texture.has_value()) {
        throw std::runtime_error(std::format("Failed to get texture for sprite: {}", sprite));
    }
    return maybe_texture.value();
}

Sprite::Sprite(GameObject& parent, const std::string& sprite, const Color color, const int flip_x, const int flip_y, const int sorting_layer, const int ordering_layer)
    : Component(parent),
    texture_(get_texture_for(sprite)),
    flip_x_(flip_x),
    flip_y_(flip_y),
    sorting_layer_(sorting_layer),
    ordering_layer_(ordering_layer),
    color_(color) {
}

int Sprite::flip_x() const {
    return flip_x_;
}
Sprite& Sprite::flip_x(const int val) {
    flip_x_ = val;
    return *this;
}

int Sprite::flip_y() const {
    return flip_y_;
}
Sprite& Sprite::flip_y(const int val) {
    flip_y_ = val;
    return *this;
}

int Sprite::sorting_layer() const {
    return sorting_layer_;
}
Sprite& Sprite::sorting_layer(const int val) {
    sorting_layer_ = val;
    return *this;
}

int Sprite::ordering_layer() const {
    return ordering_layer_;
}
Sprite& Sprite::ordering_layer(const int val) {
    ordering_layer_ = val;
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
Sprite& Sprite::texture(const std::string& name) {
    texture_ = get_texture_for(name);
    return *this;
}
Sprite& Sprite::texture(Texture& texture) {
    texture_ = texture;
    return *this;
}

void Sprite::update() {

}
void Sprite::on_attach() {

}
void Sprite::on_detach() {

}
void Sprite::on_serialize() {

}
void Sprite::on_deserialize() {

}