#pragma once

#include <engine/public/components/ui/ui_object.h>
#include <engine/public/util/color.h>

class UIText : public UIObject
{
public:
    UIText(
        std::string text,
        float width,
        float height,
        Point pivot,
        Point anchor,
        Scene& scene
    );

    void update(float dt) override;
    void render() const override;

    [[nodiscard]] 
    const std::string& text() const;
    UIText& text(const std::string& text);

    [[nodiscard]]
    const std::string& font_name() const;
    UIText& font_name(const std::string& font_name);

    [[nodiscard]]
    int font_size() const;
    UIText& font_size(int font_size);

    [[nodiscard]]
    Color color() const;
    UIText& color(Color color);

private:
    std::string text_;
    std::string font_name_;
    int font_size_;
    Color color_;
};