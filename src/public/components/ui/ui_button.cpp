#include <engine/public/components/ui/ui_button.h>

constexpr unsigned short default_color_value = 255;

UIButton::UIButton(
    float width,
    float height,
    Point pivot,
    Point anchor,
    Scene& scene
) : 
    UIObject(width, height, pivot, anchor, scene),
    color_(default_color_value, default_color_value, default_color_value, default_color_value),
    state_() 
{}

void UIButton::render() const {
    // TODO: implement button rendering
}

void UIButton::update(float dt) {
    // TODO: implement button state updates (hover, pressed, etc.)
}

void UIButton::add_on_click_handler(const std::function<void(UIButton&)>& handler) {
    on_click_handlers_.emplace_back(handler);
}

void UIButton::trigger_on_click() {
    for (const auto& handler : on_click_handlers_) {
        handler(*this);
    }
}

void UIButton::hover() {
    if (state_.is_disabled) {
        return;
    }

    state_.is_hovered = true;
}

void UIButton::unhover() {
    state_.is_hovered = false;
}

void UIButton::press() {
    if (state_.is_disabled) {
        return;
    }

    state_.is_pressed = true;
}

void UIButton::release() {
    if (state_.is_pressed && state_.is_hovered && !state_.is_disabled) {
        trigger_on_click();
    }

    state_.is_pressed = false;
}

void UIButton::disable() {
    state_.is_disabled = true;
}

void UIButton::enable() {
    if (!state_.is_disabled) {
        return;
    }

    state_.is_disabled = false;
}

void UIButton::focus() {
    if (state_.is_disabled) {
        return;
    }

    state_.is_focused = true;
}

void UIButton::unfocus() {
    state_.is_focused = false;
}

void UIButton::select() {
    if (state_.is_disabled) {
        return;
    }

    state_.is_selected = true;
}

void UIButton::deselect() {
    state_.is_selected = false;
}

void UIButton::reset_state() {
    state_ = UIButtonState{};
}

Color UIButton::color() const {
    return color_;
}

UIButton& UIButton::color(Color color) {
    color_ = color;
    return *this;
}

UIButtonState& UIButton::state() {
    return state_;
}

UIButton& UIButton::state(UIButtonState state) {
    state_ = state;
    return *this;
}