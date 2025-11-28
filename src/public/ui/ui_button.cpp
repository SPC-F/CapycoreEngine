#include <engine/public/ui/ui_button.h>

#include <engine/public/components/ui/text.h>
#include <engine/public/components/ui/image.h>

constexpr unsigned short default_color_value = 255;
constexpr unsigned short default_font_size = 16;

UIButton::UIButton(
    float width,
    float height,
    Point pivot,
    Point anchor,
    Scene& scene,
    const std::string& label,
    const std::string& font,
    const std::string& font_path,
    const std::string& sprite_path
) : 
    UIObject(width, height, pivot, anchor, scene),
    state_() 
{
    Color default_color{
        default_color_value,
        default_color_value,
        default_color_value,
        default_color_value
    };

    auto& sprite = add_component<Image>(
        sprite_path,
        0,
        0,
        static_cast<int>(width),
        static_cast<int>(height),
        default_color
    );

    add_component<Text>(
        label,
        font,
        font_path,
        default_font_size,
        default_color
    );
}

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

std::string UIButton::label() const {
    auto text_opt = get_component<Text>();
    if (!text_opt.has_value()) {
        return "";
    }

    auto& text = text_opt->get();
    return text.text();
}

UIButton& UIButton::label(const std::string& label) {
    auto text_opt = get_component<Text>(); 
    if (!text_opt.has_value()) {
        return *this;
    }

    auto& text = text_opt->get();
    text.text(label);
    return *this;
}

Color UIButton::label_color() const {
    auto text_opt = get_component<Text>();
    if (!text_opt.has_value()) {
        return Color{};
    }

    auto& text = text_opt->get();
    return text.color();
}

UIButton& UIButton::label_color(const Color& color) {
    auto text_opt = get_component<Text>();
    if (!text_opt.has_value()) {
        return *this;
    }

    auto& text = text_opt->get();
    text.color(color);
    return *this;
}

int UIButton::font_size() const {
    auto text_opt = get_component<Text>();
    if (!text_opt.has_value()) {
        return 0;
    }

    auto& text = text_opt->get();
    return text.font_size();
}

UIButton& UIButton::font_size(int size) {
    auto text_opt = get_component<Text>();
    if (!text_opt.has_value()) {
        return *this;
    }

    auto& text = text_opt->get();
    text.font_size(size);
    return *this;
}

Color UIButton::button_color() const {
    auto image_opt = get_component<Image>();
    if (!image_opt.has_value()) {
        return Color{};
    }

    auto& image = image_opt->get();
    return image.color();
}

UIButton& UIButton::button_color(const Color& color) {
    auto image_opt = get_component<Image>();
    if (!image_opt.has_value()) {
        return *this;
    }

    auto& image = image_opt->get();
    image.color(color);
    return *this;
}

UIButtonState& UIButton::state() {
    return state_;
}

UIButton& UIButton::state(UIButtonState state) {
    state_ = state;
    return *this;
}