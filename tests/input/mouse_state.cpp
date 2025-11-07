#include <catch2/catch_test_macros.hpp>

#include <engine/input/mouse_state.h>

TEST_CASE("mouse_button_state_reset_should_set_current_and_previous_to_false", "[MouseButtonState]") {
    MouseButtonState buttonState;

    buttonState.current = true;
    buttonState.previous = true;

    buttonState.reset();

    REQUIRE(buttonState.current == false);
    REQUIRE(buttonState.previous == false);
}

TEST_CASE("mouse_wheel_state_reset_should_reset_scroll_and_delta_values", "[MouseWheelState]") {
    MouseWheelState wheelState;

    wheelState.x_scroll = 10.0f;
    wheelState.y_scroll = 15.0f;
    wheelState.x_delta = 2.0f;
    wheelState.y_delta = 3.0f;
    wheelState.last_scroll_direction = MouseDirection::up;

    wheelState.reset();

    REQUIRE(wheelState.x_scroll == 0.0f);
    REQUIRE(wheelState.y_scroll == 0.0f);
    REQUIRE(wheelState.x_delta == 0.0f);
    REQUIRE(wheelState.y_delta == 0.0f);
    REQUIRE(wheelState.last_scroll_direction == MouseDirection::none);
}

TEST_CASE("mouse_state_reset_should_reset_position_and_deltas", "[MouseState]") {
    MouseState mouseState;

    mouseState.position = Point(100, 100);
    mouseState.delta_x = 5.0f;
    mouseState.delta_y = 5.0f;

    mouseState.reset();

    REQUIRE(mouseState.delta_x == 0.0f);
    REQUIRE(mouseState.delta_y == 0.0f);
}

TEST_CASE("mouse_state_reset_should_reset_button_states", "[MouseState]") {
    MouseState mouseState;

    mouseState.buttons[MouseButton::left].current = true;
    mouseState.buttons[MouseButton::right].previous = true;

    mouseState.reset();

    REQUIRE(mouseState.buttons[MouseButton::left].current == false);
    REQUIRE(mouseState.buttons[MouseButton::left].previous == false);
    REQUIRE(mouseState.buttons[MouseButton::right].current == false);
    REQUIRE(mouseState.buttons[MouseButton::right].previous == false);
}

TEST_CASE("mouse_state_reset_should_reset_wheel_state", "[MouseState]") {
    MouseState mouseState;

    mouseState.wheel.x_scroll = 10.0f;
    mouseState.wheel.y_scroll = 20.0f;
    mouseState.wheel.x_delta = 3.0f;
    mouseState.wheel.y_delta = 4.0f;
    mouseState.wheel.last_scroll_direction = MouseDirection::down;

    mouseState.reset();

    REQUIRE(mouseState.wheel.x_scroll == 0.0f);
    REQUIRE(mouseState.wheel.y_scroll == 0.0f);
    REQUIRE(mouseState.wheel.x_delta == 0.0f);
    REQUIRE(mouseState.wheel.y_delta == 0.0f);
    REQUIRE(mouseState.wheel.last_scroll_direction == MouseDirection::none);
}
