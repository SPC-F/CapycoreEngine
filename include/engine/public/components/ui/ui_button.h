#pragma once

#include <functional>

#include <engine/public/components/ui/ui_object.h>
#include <engine/public/util/color.h>

struct UIButtonState {
    bool is_hovered = false;
    bool is_pressed = false;
    bool is_disabled = false;
    bool is_focused = false;
    bool is_selected = false;
    bool is_checked = false;
};

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

    void add_on_click_handler(const std::function<void(UIButton&)>& handler);
    void trigger_on_click();

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
    std::vector<std::function<void(UIButton&)>> on_click_handlers_;
};