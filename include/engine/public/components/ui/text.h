#pragma once

#include <string>

#include <engine/public/component.h>
#include <engine/core/rendering/renderable.h>
#include <engine/public/util/color.h>

class Text : public Renderable {
public:
    Text(std::string text, std::string font, std::string font_path, int font_size, Color color);

    void update(float dt) override {}
    void on_serialize() override {}
    void on_deserialize() override {}

    [[nodiscard]] const std::string& text() const;
    Text& text(const std::string& text);

    [[nodiscard]] const std::string& font() const;
    Text& font(const std::string& font);

    [[nodiscard]] const std::string& font_path() const;
    Text& font_path(const std::string& font_path);

    [[nodiscard]] int font_size() const;
    Text& font_size(int font_size);

    [[nodiscard]] Color color() const;
    Text& color(Color color);

    [[nodiscard]] bool dirty() const;
    void mark_dirty(bool dirty = true);

private:
    std::string text_;
    std::string font_;
    std::string font_path_;
    int font_size_;
    Color color_;

    bool dirty_;
};