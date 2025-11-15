#include <catch2/catch_test_macros.hpp>

#include <engine/input/input_system.h>

class TestableInputSystem : public InputSystem {
public:
    using InputSystem::key_states_;
    using InputSystem::mouse_state_;
};

TEST_CASE("is_key_held_should_return_true_when_key_current_is_true", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.update();
    input.key_states_[KeyCode::a].current = true;

    REQUIRE(input.is_key_held(KeyCode::a) == true);
}

TEST_CASE("is_key_pressed_should_return_true_only_when_key_transitions_to_pressed", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::a].previous = false;
    input.key_states_[KeyCode::a].current = true;

    REQUIRE(input.is_key_pressed(KeyCode::a) == true);
}

TEST_CASE("is_key_released_should_return_true_only_when_key_transitions_to_released", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::a].previous = true;
    input.key_states_[KeyCode::a].current = false;

    REQUIRE(input.is_key_released(KeyCode::a) == true);
}

TEST_CASE("any_key_held_should_return_true_when_any_key_is_held", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::a].current = true;

    REQUIRE(input.any_key_held() == true);
}

TEST_CASE("any_key_pressed_should_return_true_when_any_key_is_pressed", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::b].current = true;
    input.key_states_[KeyCode::b].previous = false;

    REQUIRE(input.any_key_pressed() == true);
}

TEST_CASE("any_key_released_should_return_true_when_any_key_is_released", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::c].current = false;
    input.key_states_[KeyCode::c].previous = true;

    REQUIRE(input.any_key_released() == true);
}

TEST_CASE("is_mouse_held_should_return_true_when_button_current_is_true", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::left].current = true;

    REQUIRE(input.is_mouse_held(MouseButton::left) == true);
}

TEST_CASE("is_mouse_pressed_should_return_true_only_when_button_transitions_to_pressed", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::left].previous = false;
    input.mouse_state_.buttons[MouseButton::left].current = true;

    REQUIRE(input.is_mouse_pressed(MouseButton::left) == true);
}

TEST_CASE("is_mouse_released_should_return_true_only_when_button_transitions_to_released", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::left].previous = true;
    input.mouse_state_.buttons[MouseButton::left].current = false;

    REQUIRE(input.is_mouse_released(MouseButton::left) == true);
}

TEST_CASE("any_mouse_held_should_return_true_when_any_mouse_button_is_held", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::right].current = true;

    REQUIRE(input.any_mouse_held() == true);
}

TEST_CASE("any_mouse_pressed_should_return_true_when_any_mouse_button_is_pressed", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::middle].previous = false;
    input.mouse_state_.buttons[MouseButton::middle].current = true;

    REQUIRE(input.any_mouse_pressed() == true);
}

TEST_CASE("any_mouse_released_should_return_true_when_any_mouse_button_is_released", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.buttons[MouseButton::middle].previous = true;
    input.mouse_state_.buttons[MouseButton::middle].current = false;

    REQUIRE(input.any_mouse_released() == true);
}

TEST_CASE("mouse_position_should_return_current_position", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.position = Point(200, 150);

    REQUIRE(input.mouse_position().x == 200);
    REQUIRE(input.mouse_position().y == 150);
}

TEST_CASE("mouse_delta_should_return_current_delta", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.delta_x = 5.0f;
    input.mouse_state_.delta_y = -3.0f;

    auto delta = input.mouse_delta();
    REQUIRE(delta.first == 5.0f);
    REQUIRE(delta.second == -3.0f);
}

TEST_CASE("mouse_moved_should_return_true_when_delta_is_nonzero", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.delta_x = 1.0f;
    input.mouse_state_.delta_y = 0.0f;

    REQUIRE(input.mouse_moved() == true);
}

TEST_CASE("mouse_scroll_should_return_total_scroll", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.wheel.x_scroll = 10.0f;
    input.mouse_state_.wheel.y_scroll = 5.0f;

    auto scroll = input.mouse_scroll();
    REQUIRE(scroll.first == 10.0f);
    REQUIRE(scroll.second == 5.0f);
}

TEST_CASE("mouse_scroll_delta_should_return_delta_scroll", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.wheel.x_delta = 2.0f;
    input.mouse_state_.wheel.y_delta = -1.0f;

    auto delta = input.mouse_scroll_delta();
    REQUIRE(delta.first == 2.0f);
    REQUIRE(delta.second == -1.0f);
}

TEST_CASE("mouse_scroll_direction_should_return_last_scroll_direction", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.mouse_state_.wheel.last_scroll_direction = MouseDirection::up;

    REQUIRE(input.mouse_scroll_direction() == MouseDirection::up);
}

TEST_CASE("reset_state_should_reset_all_key_and_mouse_states", "[InputSystem]")
{
    TestableInputSystem input;

    input.key_states_[KeyCode::a].current = true;
    input.key_states_[KeyCode::a].previous = true;
    input.mouse_state_.buttons[MouseButton::left].current = true;
    input.mouse_state_.buttons[MouseButton::left].previous = true;

    input.reset_state();

    REQUIRE(input.key_states_[KeyCode::a].current == false);
    REQUIRE(input.key_states_[KeyCode::a].previous == false);
    REQUIRE(input.mouse_state_.buttons[MouseButton::left].current == false);
    REQUIRE(input.mouse_state_.buttons[MouseButton::left].previous == false);
}

TEST_CASE("update_should_move_current_to_previous_for_keys_and_mouse_buttons", "[InputSystem]")
{
    TestableInputSystem input;
    input.reset_state();

    input.key_states_[KeyCode::a].current = true;
    input.mouse_state_.buttons[MouseButton::left].current = true;

    input.update();

    REQUIRE(input.key_states_[KeyCode::a].previous == true);
    REQUIRE(input.mouse_state_.buttons[MouseButton::left].previous == true);
}
