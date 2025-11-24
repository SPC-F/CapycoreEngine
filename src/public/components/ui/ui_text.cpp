#include <engine/public/components/ui/ui_text.h>

constexpr unsigned short default_color_value = 255;
constexpr unsigned short default_font_size = 16;

UIText::UIText(
    std::string text,
    float width,
    float height,
    Point pivot,
    Point anchor,
    Scene& scene
) : 
    UIObject(width, height, pivot, anchor, scene),
    text_(std::move(text)),
    font_size_(default_font_size),
    color_(default_color_value, default_color_value, default_color_value, default_color_value)
{}

void UIText::render() const {
    // TODO: implement text rendering   
}

void UIText::update(float dt) {
    // No dynamic behavior for now
}

const std::string& UIText::text() const {
    return text_;
}

UIText& UIText::text(const std::string& text) {
    text_ = text;
    return *this;
}

const std::string& UIText::font_name() const {
    return font_name_;
}

UIText& UIText::font_name(const std::string& font_name) {
    font_name_ = font_name;
    return *this;
}

int UIText::font_size() const {
    return font_size_;
}

UIText& UIText::font_size(int font_size) {
    font_size_ = font_size;
    return *this;
}

Color UIText::color() const {
    return color_;
}

UIText& UIText::color(Color color) {
    color_ = color;
    return *this;
}