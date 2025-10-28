#pragma once

#include <optional>

#include "engine/input/key_code.h"

/**
 * @brief Interface for querying low-level keyboard input.
 *
 * Defines a unified polling API for backend-specific input providers
 * (e.g., SDL, GLFW, or platform-native implementations).
 */
class IInputProvider {
public:
    virtual ~IInputProvider() = default;

    /**
     * @brief Returns whether the specified key is currently held down.
     *
     * Suitable for continuous input such as movement or sustained actions.
     */
    virtual bool is_key_held(KeyCode key) const = 0;

    /**
     * @brief Returns true on the frame the specified key transitions to pressed.
     *
     * Supports one-shot actions such as jumping or confirming selections.
     */
    virtual bool is_key_pressed(KeyCode key) const = 0;

    /**
     * @brief Returns true on the frame the specified key transitions to released.
     *
     * Useful for detecting input releases or cancellation events.
     */
    virtual bool is_key_released(KeyCode key) const = 0;

        /**
     * @brief Returns true if any key is currently held down.
     *
     * Handy for pausing or skipping scenes on any user interaction.
     */
    virtual bool any_key_held() const = 0;

    /**
     * @brief Returns true if any key was pressed this frame.
     *
     * Used for detecting first input or simple "Press any key" prompts.
     */
    virtual bool any_key_pressed() const = 0;

    /**
     * @brief Returns true on the frame any key transitions to released.
     *
     * Useful for detecting global input releases or cancellation events.
     */
    virtual bool any_key_released() const = 0;

    /**
     * @brief Returns the time (in seconds) that a key has been held down.
     *
     * Useful for charging mechanics, input timing, or long-press detection.
     */
    virtual float key_hold_duration(KeyCode key) const = 0;

    /**
     * @brief Returns the last key pressed since the last update.
     *
     * Commonly used in keybinding configuration menus.
     */
    virtual std::optional<KeyCode> last_key_pressed() const = 0;

    /**
     * @brief Clears all key states (useful when changing scenes or focus).
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
};
