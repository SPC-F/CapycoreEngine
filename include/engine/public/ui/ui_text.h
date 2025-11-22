#pragma once

#include <engine/public/ui/ui_object.h>
#include <engine/public/util/color.h>

/**
 * @brief A UI text element.
 * 
 * UIText represents a text element in the user interface. It inherits from UIObject
 * and adds properties specific to text rendering, such as the text content, font name,
 * font size, and color.
 * 
 * In its core UIText is a game object with UIObject as its base class. The reason for this class
 * is to encapsulate text-specific functionality and state management, making it easier to
 * create and manage text elements within the UI system.
 */
class UIText : public UIObject
{
public:
    UIText(
        std::string text,
        std::string font,
        std::string font_path,
        float width,
        float height,
        Point pivot,
        Point anchor,
        Scene& scene
    );

    void update(float dt) override;

    [[nodiscard]] const std::string& text() const;
    UIText& text(const std::string& text);

    [[nodiscard]] const std::string& font() const;
    UIText& font(const std::string& font);

    [[nodiscard]] const std::string& font_path() const;
    UIText& font_path(const std::string& font_path);

    [[nodiscard]] int font_size() const;
    UIText& font_size(int font_size);

    [[nodiscard]] Color color() const;
    UIText& color(Color color);

private:
    std::string text_;
    std::string font_;
    std::string font_path_;
    int font_size_;
    Color color_;
};