#include <algorithm>

#include <engine/input/input_system.h>

bool InputSystem::is_key_held(KeyCode key) const
{
    auto it = key_states_.find(key);
    return it != key_states_.end() &&
        it->second.current;
}

bool InputSystem::is_key_pressed(KeyCode key) const
{
    auto it = key_states_.find(key);
    return it != key_states_.end() &&
        it->second.current && !it->second.previous;
}

bool InputSystem::is_key_released(KeyCode key) const
{
    auto it = key_states_.find(key);
    return it != key_states_.end() &&
        !it->second.current && it->second.previous;
}

bool InputSystem::any_key_held() const
{
    return std::any_of(key_states_.begin(), key_states_.end(),
        [](const auto& kv) { return kv.second.current; });
}

bool InputSystem::any_key_pressed() const
{
    return std::any_of(key_states_.begin(), key_states_.end(),
        [](const auto& kv) { return kv.second.current && !kv.second.previous; });
}

bool InputSystem::any_key_released() const
{
    return std::any_of(key_states_.begin(), key_states_.end(),
        [](const auto& kv) { return !kv.second.current && kv.second.previous; });
}

float InputSystem::key_hold_duration(KeyCode /*key*/) const
{
    // TODO: More complex logic; implement later.
    return 0.0f;
}

std::optional<KeyCode> InputSystem::last_key_pressed() const
{
    // TODO: More complex logic; implement later.
    return std::nullopt;
}

bool InputSystem::is_mouse_held(MouseButton button) const
{
    auto it = mouse_state_.buttons.find(button);
    return it != mouse_state_.buttons.end() &&
        it->second.current;
}

bool InputSystem::is_mouse_pressed(MouseButton button) const
{
    auto it = mouse_state_.buttons.find(button);
    return it != mouse_state_.buttons.end() &&
        it->second.current && !it->second.previous;
}

bool InputSystem::is_mouse_released(MouseButton button) const
{
    auto it = mouse_state_.buttons.find(button);
    return it != mouse_state_.buttons.end() &&
        !it->second.current && it->second.previous;
}

bool InputSystem::any_mouse_held() const
{
    return std::any_of(mouse_state_.buttons.begin(), mouse_state_.buttons.end(),
        [](const auto& kv) { return kv.second.current; });
}

bool InputSystem::any_mouse_pressed() const
{
    return std::any_of(mouse_state_.buttons.begin(), mouse_state_.buttons.end(),
        [](const auto& kv) { return kv.second.current && !kv.second.previous; });
}

bool InputSystem::any_mouse_released() const
{
    return std::any_of(mouse_state_.buttons.begin(), mouse_state_.buttons.end(),
        [](const auto& kv) { return !kv.second.current && kv.second.previous; });
}

size_t InputSystem::mouse_hold_duration(MouseButton /*button*/) const
{
    // TODO: More complex logic; implement later.
    return 0;
}

std::optional<MouseButton> InputSystem::last_mouse_pressed() const
{
    // TODO: More complex logic; implement later.
    return std::nullopt;
}

const Point InputSystem::mouse_position() const
{
    return mouse_state_.position;
}

const std::pair<float, float> InputSystem::mouse_delta() const
{
    return { mouse_state_.delta_x, mouse_state_.delta_y };
}

bool InputSystem::mouse_moved() const
{
    return mouse_state_.delta_x != 0.0f || mouse_state_.delta_y != 0.0f;
}

const std::pair<float, float> InputSystem::mouse_scroll() const
{
    return { mouse_state_.wheel.x_scroll, mouse_state_.wheel.y_scroll };
}

const std::pair<float, float> InputSystem::mouse_scroll_delta() const
{
    return { mouse_state_.wheel.x_delta, mouse_state_.wheel.y_delta };
}

MouseDirection InputSystem::mouse_scroll_direction() const
{
    return mouse_state_.wheel.last_scroll_direction;
}

void InputSystem::reset_state()
{
    for (auto& [_, state] : key_states_)
        state.reset();
    mouse_state_.reset();
}

void InputSystem::update()
{
    for (auto& [_, state] : key_states_)
        state.previous = state.current;

    for (auto& [_, state] : mouse_state_.buttons)
        state.previous = state.current;

    input().update(key_states_, mouse_state_);
}
