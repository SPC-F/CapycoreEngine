#include <engine/public/ui/interactable/ui_interactable.h>

#include <engine/core/engine.h>
#include <engine/core/system/system_service.h>
#include <engine/input/input_manager.h>
#include <engine/input/input_system.h>
#include <engine/public/transform.h>

UIInteractable::UIInteractable(
    Scene& scene,
    float width,
    float height,
    Point pivot,
    Point anchor
)
: UIObject(scene, width, height, pivot, anchor),
  interaction_state_()
{
}

UIInteractionState& UIInteractable::state() {
    return interaction_state_;
}

const UIInteractionState& UIInteractable::state() const {
    return interaction_state_;
}

bool UIInteractable::in_range(const Point& mouse_pos) const {
    Vector3 pos = transform().position();
    
    return (
        pos.x <= mouse_pos.x &&
        mouse_pos.x <= pos.x + width() &&
        pos.y <= mouse_pos.y &&
        mouse_pos.y <= pos.y + height()
    );
}

void UIInteractable::disable() {
    interaction_state_.disabled = true;
}

void UIInteractable::enable() {
    interaction_state_.disabled = false;
}

void UIInteractable::update_interaction(float dt) {
    auto& input_manager = Engine::instance().services->get_service<InputManager>().get();
    const auto& input = input_manager.provider();

    Point mouse = input.mouse_position();
    bool over = in_range(mouse);

    if (!interaction_state_.disabled) {
        if (over && !interaction_state_.hovered) {
            interaction_state_.hovered = true;
            on_hover();
            for (auto& h : on_hover_handlers_) h(*this);
        }
        if (!over && interaction_state_.hovered) {
            interaction_state_.hovered = false;
            on_unhover();
            for (auto& h : on_unhover_handlers_) h(*this);
        }
    }

    if (!interaction_state_.disabled) {
        if (over && input.is_mouse_pressed(MouseButton::left)) {
            interaction_state_.pressed = true;
            on_press();
            for (auto& h : on_press_handlers_) h(*this);
        }

        if (interaction_state_.pressed && input.is_mouse_held(MouseButton::left)) {
            interaction_state_.held = true;
            on_hold();
        }

        if (interaction_state_.pressed && input.is_mouse_released(MouseButton::left)) {
            interaction_state_.pressed = false;
            interaction_state_.held = false;
            on_release();
            for (auto& h : on_release_handlers_) h(*this);
        }
    }

    if (input.is_mouse_pressed(MouseButton::left)) {
        if (over && !interaction_state_.disabled) {
            if (!interaction_state_.focused) {
                interaction_state_.focused = true;
                on_focus();
                for (auto& h : on_focus_handlers_) h(*this);
            }
        } else {
            if (interaction_state_.focused) {
                interaction_state_.focused = false;
                on_unfocus();
                for (auto& h : on_unfocus_handlers_) h(*this);
            }
        }
    }
}

void UIInteractable::add_on_hover(const std::function<void(UIInteractable&)>& handler) {
    on_hover_handlers_.push_back(handler);
}

void UIInteractable::add_on_unhover(const std::function<void(UIInteractable&)>& handler) {
    on_unhover_handlers_.push_back(handler);
}

void UIInteractable::add_on_press(const std::function<void(UIInteractable&)>& handler) {
    on_press_handlers_.push_back(handler);
}

void UIInteractable::add_on_release(const std::function<void(UIInteractable&)>& handler) {
    on_release_handlers_.push_back(handler);
}

void UIInteractable::add_on_focus(const std::function<void(UIInteractable&)>& handler) {
    on_focus_handlers_.push_back(handler);
}

void UIInteractable::add_on_unfocus(const std::function<void(UIInteractable&)>& handler) {
    on_unfocus_handlers_.push_back(handler);
}
