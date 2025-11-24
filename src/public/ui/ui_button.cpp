#include <engine/public/ui/ui_button.h>

#include <iostream>

#include <engine/public/components/ui/text.h>
#include <engine/public/components/ui/image.h>

#include <engine/core/engine.h>
#include <engine/input/input_manager.h>
#include <engine/input/input_system.h>

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

void UIButton::update(float dt) {
    auto& input_manager = Engine::instance().services->get_service<InputManager>().get();
    const auto& input_provider = input_manager.provider();

    auto mouse_pos = input_provider.mouse_position();
    bool is_mouse_over = this->transform().position().x <= mouse_pos.x &&
                         mouse_pos.x <= this->transform().position().x + this->width() &&
                         this->transform().position().y <= mouse_pos.y &&
                         mouse_pos.y <= this->transform().position().y + this->height();
    
    if (is_mouse_over) {
        auto darken = [this](UIButton& btn) {
            button_color(Color{200, 200, 200, 255});
        };

        hover(darken);
    } 
    else {
        auto lighten = [this](UIButton& btn) {
            button_color(Color{255, 255, 255, 255});
        };

        unhover(lighten);
    }

    if (is_mouse_over && input_provider.is_mouse_pressed(MouseButton::left)) {
        press();
    }
    
    if (is_mouse_over && state_.is_pressed && input_provider.is_mouse_released(MouseButton::left)) {
        release();
    }
}

void UIButton::add_on_press(const std::function<void(UIButton&)>& handler) {
    on_press_handlers_.emplace_back(handler);
}

void UIButton::trigger_on_press() {
    for (const auto& handler : on_press_handlers_) {
        handler(*this);
    }
}

void UIButton::hover(std::function<void(UIButton&)> on_hovered) {
    if (state_.is_disabled) {
        return;
    }

    state_.is_hovered = true;
    if (on_hovered) {
        on_hovered(*this);
    }
}

void UIButton::unhover(std::function<void(UIButton&)> on_unhovered) {
    state_.is_hovered = false;

    if (on_unhovered) {
        on_unhovered(*this);
    }
}

void UIButton::press(std::function<void(UIButton&)> on_pressed) {
    if (state_.is_disabled) {
        return;
    }

    state_.is_pressed = true;
    
    if (on_pressed) {
        on_pressed(*this);
    }
}

void UIButton::release(std::function<void(UIButton&)> on_released) {
    if (state_.is_pressed && state_.is_hovered && !state_.is_disabled) {
        trigger_on_press();
    }

    state_.is_pressed = false;

    if (on_released) {
        on_released(*this);
    }
}

void UIButton::disable(std::function<void(UIButton&)> on_disabled) {
    state_.is_disabled = true;

    if (on_disabled) {
        on_disabled(*this);
    }
}

void UIButton::enable(std::function<void(UIButton&)> on_enabled) {
    if (!state_.is_disabled) {
        return;
    }
    
    state_.is_disabled = false;

    if (on_enabled) {
        on_enabled(*this);
    }
}

void UIButton::focus(std::function<void(UIButton&)> on_focused) {
    if (state_.is_disabled) {
        return;
    }

    state_.is_focused = true;

    if (on_focused) {
        on_focused(*this);
    }
}

void UIButton::unfocus(std::function<void(UIButton&)> on_unfocused) {
    state_.is_focused = false;

    if (on_unfocused) {
        on_unfocused(*this);
    }
}

void UIButton::select(std::function<void(UIButton&)> on_selected) {
    if (state_.is_disabled) {
        return;
    }

    state_.is_selected = true;

    if (on_selected) {
        on_selected(*this);
    }
}

void UIButton::deselect(std::function<void(UIButton&)> on_deselected) {
    state_.is_selected = false;

    if (on_deselected) {
        on_deselected(*this);
    }
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