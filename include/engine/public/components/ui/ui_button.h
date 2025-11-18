#pragma once

#include <functional>

#include <engine/public/components/ui/ui_object.h>
#include <engine/public/util/color.h>

/**
 * @brief Structure representing the state of a UIButton.
 * 
 * This structure holds various boolean flags that represent the current state of a UIButton,
 * such as whether it is hovered, pressed, disabled, focused, selected, or checked.
 */
struct UIButtonState {
    bool is_hovered = false;
    bool is_pressed = false;
    bool is_disabled = false;
    bool is_focused = false;
    bool is_selected = false;
    bool is_checked = false;
};

/**
 * @brief A UI button element.
 * 
 * UIButton represents a button element in the user interface. It inherits from UIObject
 * and adds properties and behaviors specific to buttons, such as click handling,
 * hover state, press state, and visual appearance.
 */
class UIButton : public UIObject
{
public:
    UIButton(
        float width,
        float height,
        Point pivot,
        Point anchor,
        Scene& scene
    );

    void update(float dt) override;
    void render() const override;

    void add_on_press(const std::function<void(UIButton&)>& handler);
    void trigger_on_press();

    void hover();
    void unhover();

    void press();
    void release();

    void disable();
    void enable();

    void focus();
    void unfocus();

    void select();
    void deselect();

    void reset_state();

    [[nodiscard]]
    Color color() const;
    UIButton& color(Color color);

    [[nodiscard]]
    UIButtonState& state();
    UIButton& state(UIButtonState state);

private:
    Color color_;
    UIButtonState state_;
    std::vector<std::function<void(UIButton&)>> on_press_handlers_;
};