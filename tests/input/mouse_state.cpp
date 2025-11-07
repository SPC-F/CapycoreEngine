#include <catch2/catch_test_macros.hpp>

#include <engine/input/mouse_state.h>

TEST_CASE("mouse_button_state_reset_should_set_current_and_previous_to_false", "[MouseButtonState]") {
    MouseButtonState button_state;

    button_state.current = true;
    button_state.previous = true;

    button_state.reset();

    REQUIRE(button_state.current == false);
    REQUIRE(button_state.previous == false);
}

TEST_CASE("mouse_wheel_state_reset_should_reset_scroll_and_delta_values", "[MouseWheelState]") {
    MouseWheelState wheel_state;

    wheel_state.x_scroll = 10.0f;
    wheel_state.y_scroll = 15.0f;
    wheel_state.x_delta = 2.0f;
    wheel_state.y_delta = 3.0f;
    wheel_state.last_scroll_direction = MouseDirection::up;

    wheel_state.reset();

    REQUIRE(wheel_state.x_scroll == 0.0f);
    REQUIRE(wheel_state.y_scroll == 0.0f);
    REQUIRE(wheel_state.x_delta == 0.0f);
    REQUIRE(wheel_state.y_delta == 0.0f);
    REQUIRE(wheel_state.last_scroll_direction == MouseDirection::none);
}

TEST_CASE("mouse_state_reset_should_reset_position_and_deltas", "[MouseState]") {
    MouseState mouse_state;

    mouse_state.position = Point(100, 100);
    mouse_state.delta_x = 5.0f;
    mouse_state.delta_y = 5.0f;

    mouse_state.reset();

    REQUIRE(mouse_state.delta_x == 0.0f);
    REQUIRE(mouse_state.delta_y == 0.0f);
}

TEST_CASE("mouse_state_reset_should_reset_button_states", "[MouseState]") {
    MouseState mouse_state;

    mouse_state.buttons[MouseButton::left].current = true;
    mouse_state.buttons[MouseButton::right].previous = true;

    mouse_state.reset();

    REQUIRE(mouse_state.buttons[MouseButton::left].current == false);
    REQUIRE(mouse_state.buttons[MouseButton::left].previous == false);
    REQUIRE(mouse_state.buttons[MouseButton::right].current == false);
    REQUIRE(mouse_state.buttons[MouseButton::right].previous == false);
}

TEST_CASE("mouse_state_reset_should_reset_wheel_state", "[MouseState]") {
    MouseState mouse_state;

    mouse_state.wheel.x_scroll = 10.0f;
    mouse_state.wheel.y_scroll = 20.0f;
    mouse_state.wheel.x_delta = 3.0f;
    mouse_state.wheel.y_delta = 4.0f;
    mouse_state.wheel.last_scroll_direction = MouseDirection::down;

    mouse_state.reset();

    REQUIRE(mouse_state.wheel.x_scroll == 0.0f);
    REQUIRE(mouse_state.wheel.y_scroll == 0.0f);
    REQUIRE(mouse_state.wheel.x_delta == 0.0f);
    REQUIRE(mouse_state.wheel.y_delta == 0.0f);
    REQUIRE(mouse_state.wheel.last_scroll_direction == MouseDirection::none);
}
