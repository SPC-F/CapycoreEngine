#include <engine/public/components/ui/image.h>

#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>

std::reference_wrapper<Texture> Image::get_texture_for(const std::string& sprite) {
    auto& service = Engine::instance().services->get_service<AssetService>().get();
    auto maybe_texture = service.try_get_texture(sprite);

    if (!maybe_texture.has_value()) {
        return service.get_default_texture();
    }

    return maybe_texture->get();
}

Image::Image(
    const std::string image,
    int flip_x,
    int flip_y,
    int width,
    int height,
    Color color
) : texture_(get_texture_for(image)),
    flip_x_(flip_x),
    flip_y_(flip_y),
    width_(width),
    height_(height),
    color_(color) {
    add_on_attach([this](Component& comp) {
        this->set_render_strategy(comp);
    });
}

void Image::update(float dt) {}

void Image::on_serialize() {}

void Image::on_deserialize() {}

int Image::flip_x() const {
    return flip_x_;
}

Image& Image::flip_x(int val) {
    flip_x_ = val;
    return *this;
}

int Image::flip_y() const {
    return flip_y_;
}

Image& Image::flip_y(int val) {
    flip_y_ = val;
    return *this;
}

int Image::width() const {
    return width_;
}

Image& Image::width(int val) {
    width_ = val;
    return *this;
}

int Image::height() const {
    return height_;
}

Image& Image::height(int val) {
    height_ = val;
    return *this;
}

Color Image::color() const {
    return color_;
}

Image& Image::color(Color color) {
    color_ = color;
    return *this;
}

const Texture& Image::texture() const {
    return texture_;
}

Image& Image::texture(const std::string& name) {
    texture_ = get_texture_for(name);
    return *this;
}

Image& Image::texture(Texture& texture) {
    texture_ = texture;
    return *this;
}