#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <utility>

#include <engine/input/i_input.h>
#include <engine/input/key_code.h>
#include <engine/input/mouse_button.h>
#include <engine/input/mouse_state.h>
#include <engine/public/util/point.h>

/**
 * @brief Interface for querying low-level keyboard input.
 *
 * Defines a unified polling API for backend-specific input providers
 * (e.g., SDL, GLFW, or platform-native implementations).
 */
class IInputProvider {
public:
    virtual ~IInputProvider() = default;

    void set_input(std::unique_ptr<IInput> input);
    [[nodiscard]] IInput& input() const;

    /**
     * @brief Returns whether the specified key is currently held down.
     *
     * Suitable for continuous input such as movement or sustained actions.
     */
    [[nodiscard]] virtual bool is_key_held(KeyCode key) const = 0;

    /**
     * @brief Returns true on the frame the specified key transitions to pressed.
     *
     * Supports one-shot actions such as jumping or confirming selections.
     */
    [[nodiscard]] virtual bool is_key_pressed(KeyCode key) const = 0;

    /**
     * @brief Returns true on the frame the specified key transitions to released.
     *
     * Useful for detecting input releases or cancellation events.
     */
    [[nodiscard]] virtual bool is_key_released(KeyCode key) const = 0;

        /**
     * @brief Returns true if any key is currently held down.
     *
     * Handy for pausing or skipping scenes on any user interaction.
     */
    [[nodiscard]] virtual bool any_key_held() const = 0;

    /**
     * @brief Returns true if any key was pressed this frame.
     *
     * Used for detecting first input or simple "Press any key" prompts.
     */
    [[nodiscard]] virtual bool any_key_pressed() const = 0;

    /**
     * @brief Returns true on the frame any key transitions to released.
     *
     * Useful for detecting global input releases or cancellation events.
     */
    [[nodiscard]] virtual bool any_key_released() const = 0;

    /**
     * @brief Returns the time (in seconds) that a key has been held down.
     *
     * Useful for charging mechanics, input timing, or long-press detection.
     */
    [[nodiscard]] virtual float key_hold_duration(KeyCode key) const = 0;

    /**
     * @brief Returns the last key pressed since the last update.
     *
     * Commonly used in keybinding configuration menus.
     */
    [[nodiscard]] virtual std::optional<KeyCode> last_key_pressed() const = 0;

    /** @brief Returns whether the specified mouse button is currently held down.
     *
     * Suitable for continuous input such as aiming or dragging.
     */
    [[nodiscard]] virtual bool is_mouse_held(MouseButton button) const = 0;

    /** @brief Returns true on the frame the specified mouse button transitions to pressed.
     *
     * Supports one-shot actions such as firing or selecting.
     */
    [[nodiscard]] virtual bool is_mouse_pressed(MouseButton button) const = 0;

    /** @brief Returns true on the frame the specified mouse button transitions to released.
     *
     * Useful for detecting clicks, drops, or interaction ends.
     */
    [[nodiscard]] virtual bool is_mouse_released(MouseButton button) const = 0;

    /** @brief Returns true if any mouse button is currently held down.
     *
     * Useful for generic interaction detection or camera controls.
     */
    [[nodiscard]] virtual bool any_mouse_held() const = 0;

    /** @brief Returns true if any mouse button was pressed this frame.
     *
     * Detects first mouse interaction or UI click triggers.
     */
    [[nodiscard]] virtual bool any_mouse_pressed() const = 0;

    /** @brief Returns true on the frame any mouse button transitions to released.
     *
     * Helps detect end of drag or click-release events.
     */
    [[nodiscard]] virtual bool any_mouse_released() const = 0;

    /** @brief Returns the time (in milliseconds) that a mouse button has been held down.
     *
     * Enables charge actions, long-click detection, or drag timing.
     */
    [[nodiscard]] virtual size_t mouse_hold_duration(MouseButton button) const = 0;

    /** @brief Returns the last mouse button pressed since the last update.
     *
     * Useful for input remapping or UI debugging.
     */
    [[nodiscard]] virtual std::optional<MouseButton> last_mouse_pressed() const = 0;

    /**
     * @brief Returns the current mouse cursor position (X, Y) in window coordinates.
     *
     * Useful for UI interaction, cursor tracking, or camera control.
     */
    [[nodiscard]] virtual const Point& mouse_position() const = 0;

    // TODO: Replace std::pair<float, float> with a dedicated Vector2 type for clearer semantics and math operations.
    /**
     * @brief Returns the change in mouse position since the last frame (deltaX, deltaY).
     *
     * Can be used to detect relative movement for camera rotation or drag operations.
     */
    [[nodiscard]] virtual std::pair<float, float> mouse_delta() const = 0;

    /**
     * @brief Returns true if the mouse moved during the current frame.
     *
     * Useful for conditional logic that only triggers when the cursor changes position.
     */
    [[nodiscard]] virtual bool mouse_moved() const = 0;

    // TODO: Replace std::pair<float, float> with a dedicated Vector2 type for clearer semantics and math operations.
    /**
     * @brief Returns the accumulated scroll offset along X and Y axes.
     *
     * Useful for continuous scrolling or zooming interactions.
     */
    [[nodiscard]] virtual std::pair<float, float> mouse_scroll() const = 0;

    // TODO: Replace std::pair<float, float> with a dedicated Vector2 type for clearer semantics and math operations.
    /**
     * @brief Returns the scroll delta since the last frame (deltaX, deltaY).
     *
     * Useful for detecting incremental scroll events for UI or gameplay.
     */
    [[nodiscard]] virtual std::pair<float, float> mouse_scroll_delta() const = 0;

    /**
     * @brief Returns the last detected scroll direction as an integer enum.
     *
     * Can be used to determine the general scroll direction (up, down, left, right).
     */
    [[nodiscard]] virtual MouseDirection mouse_scroll_direction() const = 0;

    /**
     * @brief Clears all input states (useful when changing scenes or focus).
     *
     * Ensures stale input doesn’t carry over between menus or gameplay.
     */
    virtual void reset_state() = 0;

    /**
     * @brief Polls the underlying system for input updates.
     *
     * Usually called once per frame before querying input state.
     */
    virtual void update() = 0;

private:
    std::unique_ptr<IInput> input_;
};
