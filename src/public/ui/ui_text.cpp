#include <engine/public/ui/ui_text.h>

#include <engine/public/components/ui/text.h>

constexpr unsigned short default_color_value = 255;
constexpr unsigned short default_font_size = 16;

UIText::UIText(
    std::string text,
    std::string font,
    std::string font_path,
    float width,
    float height,
    Point pivot,
    Point anchor,
    Scene& scene
) : 
    UIObject(width, height, pivot, anchor, scene),
    text_(std::move(text)),
    font_(std::move(font)),
    font_path_(std::move(font_path)),
    font_size_(default_font_size),
    color_(default_color_value, default_color_value, default_color_value, default_color_value)
{
    add_component<Text>(
        text_,
        font_,
        font_path_,
        font_size_,
        color_
    );
}

void UIText::update(float dt) {}

const std::string& UIText::text() const {
    return text_;
}

UIText& UIText::text(const std::string& text) {
    text_ = text;
    auto text_component = get_component<Text>();
    
    if (text_component.has_value()) {
        text_component->get().text(text_);
    }

    return *this;
}

const std::string& UIText::font() const {
    return font_;
}

UIText& UIText::font(const std::string& font) {
    font_ = font;

    auto text_component = get_component<Text>();

    if (text_component.has_value()) {
        text_component->get().font(font_);
    }

    return *this;
}

const std::string& UIText::font_path() const {
    return font_path_;
}

UIText& UIText::font_path(const std::string& font_path) {
    font_path_ = font_path;

    auto text_component = get_component<Text>();

    if (text_component.has_value()) {
        text_component->get().font_path(font_path_);
    }

    return *this;
}

int UIText::font_size() const {
    return font_size_;
}

UIText& UIText::font_size(int font_size) {
    font_size_ = font_size;

    auto text_component = get_component<Text>();

    if (text_component.has_value()) {
        text_component->get().font_size(font_size_);
    }

    return *this;
}

Color UIText::color() const {
    return color_;
}

UIText& UIText::color(Color color) {
    color_ = color;

    auto text_component = get_component<Text>();
    
    if (text_component.has_value()) {
        text_component->get().color(color_);
    }
    
    return *this;
}