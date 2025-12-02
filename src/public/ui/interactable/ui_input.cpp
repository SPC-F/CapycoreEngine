#include <engine/core/engine.h>
#include <engine/core/system/system_service.h>
#include <engine/input/input_manager.h>
#include <engine/input/input_system.h>
#include <engine/public/components/ui/image.h>
#include <engine/public/components/ui/text.h>
#include <engine/public/ui/interactable/ui_input.h>

constexpr unsigned short default_color_value = 255;
constexpr unsigned short default_font_size = 16;

UIInput::UIInput(Scene& scene, float width, float height, Point pivot,
                 Point anchor, const std::string& placeholder,
                 const std::string& font, const std::string& font_path,
                 const std::string& sprite_path)
    : UIInteractable(scene, width, height, pivot, anchor) {
  Color default_color{default_color_value, default_color_value,
                      default_color_value, default_color_value};

  int flip_x = 0;
  int flip_y = 0;
  add_component<Image>(sprite_path, flip_x, flip_y, static_cast<int>(width),
                       static_cast<int>(height), default_color);

  add_component<Text>(placeholder, font, font_path, default_font_size,
                      default_color);
}

void UIInput::update(float dt) {
  update_interaction(dt);

  if (!state().focused || state().disabled) {
    return;
  }

  auto& input_manager =
      Engine::instance().services->get_service<InputManager>().get();
  const auto& input = input_manager.provider();

  if (input.is_key_pressed(KeyCode::backspace)) {
    if (!content_.empty()) {
      content_.pop_back();
      text(content_);
      trigger_on_text_changed();
    }

    return;
  }

  auto key = input.get_pressed_key();
  if (key == KeyCode::unknown) {
    return;
  }

  bool text_outside_box = font_size() * static_cast<int>(content_.length()) >
                          static_cast<int>(width()) * 2;
  if (text_outside_box) {
    return;
  }

  bool copy_paste_held = (input.is_key_held(KeyCode::left_control) ||
                          input.is_key_held(KeyCode::right_control));
  if (copy_paste_held) {
    if (key == KeyCode::v) {
      auto& system_service =
          Engine::instance().services->get_service<SystemService>().get();
      std::string clipboard_text = system_service.get_clipboard_text();
      content_ += clipboard_text;

      text(content_);
      trigger_on_text_changed();

      return;
    }
  }

  bool shift_held = input.is_key_held(KeyCode::left_shift) ||
                    input.is_key_held(KeyCode::right_shift);
  if (key >= KeyCode::a && key <= KeyCode::z) {
    char c = (shift_held ? 'A' : 'a') + static_cast<int>(key) -
             static_cast<int>(KeyCode::a);  // NOLINT
    content_ += c;

    text(content_);
    trigger_on_text_changed();

    return;
  }

  if (key >= KeyCode::num_0 && key <= KeyCode::num_9) {
    char c = '0' + static_cast<int>(key) -
             static_cast<int>(KeyCode::num_0);  // NOLINT
    content_ += c;

    text(content_);
    trigger_on_text_changed();

    return;
  }
}

void UIInput::on_hover() {
  input_color(Color{200, 200, 200, 255});  // NOLINT

  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  system_service.set_cursor_to_ibeam();
}

void UIInput::on_unhover() {
  input_color(Color{255, 255, 255, 255});  // NOLINT

  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  system_service.set_cursor_to_arrow();
}

void UIInput::on_focus() {
  trigger_on_focus();
  input_color(Color{220, 220, 220, 255});  // NOLINT
}

void UIInput::on_unfocus() {
  trigger_on_unfocus();
  input_color(Color{255, 255, 255, 255});  // NOLINT
}

void UIInput::add_on_focus(const std::function<void(UIInput&)>& handler) {
  on_focus_handlers_.push_back(handler);
}

void UIInput::add_on_unfocus(const std::function<void(UIInput&)>& handler) {
  on_unfocus_handlers_.push_back(handler);
}

void UIInput::add_on_text_changed(
    const std::function<void(UIInput&)>& handler) {
  on_text_changed_handlers_.push_back(handler);
}

void UIInput::trigger_on_focus() {
  for (auto& h : on_focus_handlers_) h(*this);
}

void UIInput::trigger_on_unfocus() {
  for (auto& h : on_unfocus_handlers_) h(*this);
}

void UIInput::trigger_on_text_changed() {
  for (auto& h : on_text_changed_handlers_) h(*this);
}

std::string UIInput::text() const { return content_; }

UIInput& UIInput::text(const std::string& value) {
  content_ = value;
  auto txt = get_component<Text>();

  if (txt) {
    txt->get().text(value);
  }

  return *this;
}

Color UIInput::text_color() const {
  auto txt = get_component<Text>();

  if (!txt) {
    return Color{};
  }

  return txt->get().color();
}

UIInput& UIInput::text_color(const Color& color) {
  auto txt = get_component<Text>();

  if (txt) {
    txt->get().color(color);
  }

  return *this;
}

int UIInput::font_size() const {
  auto txt = get_component<Text>();

  if (!txt) {
    return 0;
  }

  return txt->get().font_size();
}

UIInput& UIInput::font_size(int size) {
  auto txt = get_component<Text>();

  if (txt) {
    txt->get().font_size(size);
  }

  return *this;
}

Color UIInput::input_color() const {
  auto img = get_component<Image>();

  if (!img) {
    return Color{};
  }

  return img->get().color();
}

UIInput& UIInput::input_color(const Color& color) {
  auto img = get_component<Image>();

  if (img) {
    img->get().color(color);
  }

  return *this;
}