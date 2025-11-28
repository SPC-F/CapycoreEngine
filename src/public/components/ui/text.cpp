#include <engine/public/components/ui/text.h>

Text::Text(std::string text, std::string font, std::string font_path, int font_size, Color color)
    : text_(std::move(text)),
      font_(std::move(font)),
      font_path_(std::move(font_path)),
      font_size_(font_size),
      color_(color),
      dirty_(true) {
  add_on_attach([this](Component& comp) {
      this->set_render_strategy(comp);
  });
}

const std::string& Text::text() const {
    return text_;
}

Text& Text::text(const std::string& text) {
    text_ = text;
    mark_dirty();
    return *this;
}

const std::string& Text::font() const {
    return font_;
}

Text& Text::font(const std::string& font) {
    font_ = font;
    mark_dirty();
    return *this;
}

const std::string& Text::font_path() const {
    return font_path_;
}

Text& Text::font_path(const std::string& font_path) {
    font_path_ = font_path;
    mark_dirty();
    return *this;
}

int Text::font_size() const {
    return font_size_;
}

Text& Text::font_size(int font_size) {
    font_size_ = font_size;
    mark_dirty();
    return *this;
}

Color Text::color() const {
    return color_;
}

Text& Text::color(Color color) {
    color_ = color;
    mark_dirty();
    return *this;
}

bool Text::dirty() const {
    return dirty_;
}

void Text::mark_dirty(bool dirty) {
    dirty_ = dirty;
}