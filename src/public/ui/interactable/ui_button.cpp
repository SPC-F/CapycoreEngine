#include <engine/public/ui/interactable/ui_button.h>

#include <engine/public/components/ui/text.h>
#include <engine/public/components/ui/image.h>

constexpr unsigned short default_color_value = 255;
constexpr unsigned short default_font_size   = 16;

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
)
: UIInteractable(width, height, pivot, anchor, scene)
{
    Color default_color{
        default_color_value,
        default_color_value,
        default_color_value,
        default_color_value
    };

    int flip_x = 0;
    int flip_y = 0;
    add_component<Image>(sprite_path, flip_x, flip_y, static_cast<int>(width), static_cast<int>(height), default_color);
    add_component<Text>(label, font, font_path, default_font_size, default_color);
}

void UIButton::update(float dt) {
    update_interaction(dt);
}

void UIButton::on_hover() {
    button_color(Color{200, 200, 200, 255});
}

void UIButton::on_unhover() {
    button_color(Color{255, 255, 255, 255});
}

void UIButton::on_press() {
    button_color(Color{180, 180, 180, 255});
}

void UIButton::on_release() {
    button_color(Color{255, 255, 255, 255});

    if (state().hovered && !state().disabled) {
        trigger_on_press();
    }
}

void UIButton::add_on_press(const std::function<void(UIButton&)>& handler) {
    on_press_handlers_.push_back(handler);
}

void UIButton::trigger_on_press() {
    for (auto& handler : on_press_handlers_) {
        handler(*this);
    }
}

std::string UIButton::label() const {
    auto text_opt = get_component<Text>();
    
    if (!text_opt.has_value()) {
        return "";
    }

    return text_opt->get().text();
}

UIButton& UIButton::label(const std::string& label) {
    auto text_opt = get_component<Text>();
    
    if (text_opt) {
        text_opt->get().text(label);
    }
    
    return *this;
}

Color UIButton::label_color() const {
    auto text_opt = get_component<Text>();
    
    if (!text_opt) {
        return Color{};
    }
    
    return text_opt->get().color();
}

UIButton& UIButton::label_color(const Color& color) {
    auto text_opt = get_component<Text>();
    
    if (text_opt) {
        text_opt->get().color(color);
    }
    
    return *this;
}

int UIButton::font_size() const {
    auto text_opt = get_component<Text>();
    
    if (!text_opt) {
        return 0;
    }

    return text_opt->get().font_size();
}

UIButton& UIButton::font_size(int size) {
    auto text_opt = get_component<Text>();
    
    if (text_opt) {
        text_opt->get().font_size(size);
    }

    return *this;
}

Color UIButton::button_color() const {
    auto img_opt = get_component<Image>();
    
    if (!img_opt) {
        return Color{};
    }

    return img_opt->get().color();
}

UIButton& UIButton::button_color(const Color& color) {
    auto img_opt = get_component<Image>();

    if (img_opt) {
        img_opt->get().color(color);
    }

    return *this;
}
