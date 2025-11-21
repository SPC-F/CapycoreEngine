#pragma once

/**
 * @brief Represents per-frame key transition state.
 *
 * Captures both current and previous key states to detect press and release edges.
 * Enables deterministic input evaluation independent of event timing.
 */
struct KeyState {
    bool current  = false;  ///< True if the key is down this frame.
    bool previous = false;  ///< True if the key was down in the previous frame.

    void reset();
};
