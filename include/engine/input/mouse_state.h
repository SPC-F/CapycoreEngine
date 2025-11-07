#pragma once

#include <map>

#include <engine/input/mouse_button.h>
#include <engine/public/util/point.h>

/**
 * @brief Represents the primary directions of mouse wheel movement.
 *
 * Used for querying the last scroll direction of the mouse.
 * Can be combined with per-frame deltas for precise scroll handling.
 */
enum class MouseDirection {
    none,   ///< No scroll detected (this frame).
    up,     ///< Scroll wheel moved upward (away from the user).
    down,   ///< Scroll wheel moved downward (toward the user).
    left,   ///< Scroll wheel moved leftward.
    right   ///< Scroll wheel moved rightward.
};

/**
 * @brief Represents per-frame mouse button transition state.
 *
 * Tracks both current and previous button states to detect
 * presses and releases deterministically.
 */
struct MouseButtonState {
    bool current  = false;  ///< True if the button is down this frame.
    bool previous = false;  ///< True if the button was down in the previous frame.

    void reset();
};

/**
 * @brief Represents per-frame scroll state of the mouse wheel.
 *
 * Captures accumulated position, per-frame delta, and last known direction.
 * Designed for deterministic querying without backend-specific dependencies.
 */
struct MouseWheelState {
    float x_scroll = 0.0f;  ///< Accumulated horizontal scroll position.
    float y_scroll = 0.0f;  ///< Accumulated vertical scroll position.

    float x_delta = 0.0f;   ///< Horizontal scroll change this frame.
    float y_delta = 0.0f;   ///< Vertical scroll change this frame.

    MouseDirection last_scroll_direction = MouseDirection::none;

    void reset();
    void update(float delta_x, float delta_y);
};

/**
 * @brief Aggregates all mouse-related state information.
 *
 * Encapsulates position, button states, and wheel movement into a single structure.
 * Intended for per-frame input tracking and polling through IInputProvider.
 */
struct MouseState {
    // Cursor position in window coordinates.
    Point position = Point(0, 0);

    // Cursor movement delta since last frame.
    float delta_x = 0.0f;
    float delta_y = 0.0f;

    std::map<MouseButton, MouseButtonState> buttons;
    MouseWheelState wheel;

    void reset();
    void update_position(float new_x, float new_y);
    void update_button(MouseButton button, bool pressed);
    void update_wheel(float delta_x, float delta_y);
};
