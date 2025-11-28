#pragma once

#include <functional>
#include <vector>

#include <engine/public/ui/interactable/ui_interactable.h>
#include <engine/public/components/ui/text.h>
#include <engine/public/components/ui/image.h>

/**
 * @brief A UI input field element.
 * 
 * UIInput represents a text input field in the user interface. It inherits from UIInteractable
 * and adds properties and behaviors specific to text input, such as handling focus,
 * text content, and text color.
 */
class UIInput : public UIInteractable {
public:
    UIInput(
        float width,
        float height,
        Point pivot,
        Point anchor,
        Scene& scene,
        const std::string& placeholder = "",
        const std::string& font = "",
        const std::string& font_path = "",
        const std::string& sprite_path = ""
    );

    void update(float dt) override;

    void add_on_focus(const std::function<void(UIInput&)>& handler);
    void add_on_unfocus(const std::function<void(UIInput&)>& handler);
    void add_on_text_changed(const std::function<void(UIInput&)>& handler);

    void on_hover() override;
    void on_unhover() override;
    void on_focus() override;
    void on_unfocus() override;

    [[nodiscard]] std::string text() const;
    UIInput& text(const std::string& value);

    [[nodiscard]] Color text_color() const;
    UIInput& text_color(const Color& color);

    [[nodiscard]] int font_size() const;
    UIInput& font_size(int size);

    [[nodiscard]] Color input_color() const;
    UIInput& input_color(const Color& color);

private:
    void trigger_on_focus();
    void trigger_on_unfocus();
    void trigger_on_text_changed();

    std::vector<std::function<void(UIInput&)>> on_focus_handlers_;
    std::vector<std::function<void(UIInput&)>> on_unfocus_handlers_;
    std::vector<std::function<void(UIInput&)>> on_text_changed_handlers_;

    std::string content_;
};
