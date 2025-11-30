#include <engine/public/ui/ui_image.h>

constexpr unsigned short default_color_value = 255;

UIImage::UIImage(
    Scene& scene,
    std::string image_path,
    float width,
    float height,
    Point pivot,
    Point anchor
)
: UIObject(scene, width, height, pivot, anchor),
  image_path_(std::move(image_path)),
  color_(default_color_value, default_color_value, default_color_value, default_color_value)
{
    int default_flip_x = 0;
    int default_flip_y = 0;
    add_component<Image>(image_path_, default_flip_x, default_flip_y, static_cast<int>(width), static_cast<int>(height), color_);
}

void UIImage::update(float dt) {
    // No dynamic behavior
}

const std::string& UIImage::image_path() const {
    return image_path_;
}

UIImage& UIImage::image_path(const std::string& path) {
    image_path_ = path;

    auto img_component_opt = get_component<Image>();
    if (img_component_opt.has_value()) {
        img_component_opt->get().texture(image_path_);
    }

    return *this;
}

Color UIImage::color() const {
    return color_;
}

UIImage& UIImage::color(Color color) {
    color_ = color;

    auto img_component_opt = get_component<Image>();
    if (img_component_opt.has_value()) {
        img_component_opt->get().color(color_);
    }

    return *this;
}
