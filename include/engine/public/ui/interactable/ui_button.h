#pragma once

#include <functional>

#include <engine/public/ui/interactable/ui_interactable.h>
#include <engine/public/util/color.h>

/**
 * @brief A UI button element.
 * 
 * UIButton represents a clickable button in the user interface. It inherits from UIInteractable
 * and adds properties and behaviors specific to buttons, such as handling labels,
 * button colors, and press events.
 */
class UIButton : public UIInteractable
{
public:
    UIButton(
        Scene& scene,
        float width,
        float height,
        Point pivot,
        Point anchor,
        const std::string& label = "",
        const std::string& font = "",
        const std::string& font_path = "",
        const std::string& sprite_path = ""
    );

    void update(float dt) override;

    void add_on_press(const std::function<void(UIButton&)>& handler);

    void on_hover() override;
    void on_unhover() override;

    void on_press() override;
    void on_release() override;

    [[nodiscard]] std::string label() const;
    UIButton& label(const std::string& label);

    [[nodiscard]] Color label_color() const;
    UIButton& label_color(const Color& color);

    [[nodiscard]] int font_size() const;
    UIButton& font_size(int size);

    [[nodiscard]] Color button_color() const;
    UIButton& button_color(const Color& color);

private:
    void trigger_on_press();

    std::vector<std::function<void(UIButton&)>> on_press_handlers_;
};