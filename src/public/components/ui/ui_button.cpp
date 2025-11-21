#include <engine/public/components/ui/ui_button.h>

#include <engine/public/components/ui/text.h>
#include <engine/public/components/ui/image.h>

constexpr unsigned short default_color_value = 255;

UIButton::UIButton(
    float width,
    float height,
    Point pivot,
    Point anchor,
    Scene& scene,
    std::string label,
    std::string font,
    std::string font_path,
    std::string sprite_path
) : 
    UIObject(width, height, pivot, anchor, scene),
    color_(default_color_value, default_color_value, default_color_value, default_color_value),
    state_() 
{
    auto& sprite = add_component<Image>(
        sprite_path,
        0,
        0,
        static_cast<int>(width),
        static_cast<int>(height),
        color_
    );

    add_component<Text>(
        label,
        font,
        font_path,
        16,
        color_
    );
}

void UIButton::render() const {}

void UIButton::update(float dt) {}

void UIButton::add_on_press(const std::function<void(UIButton&)>& handler) {
    on_press_handlers_.emplace_back(handler);
}

void UIButton::trigger_on_press() {
    for (const auto& handler : on_press_handlers_) {
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
        trigger_on_press();
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