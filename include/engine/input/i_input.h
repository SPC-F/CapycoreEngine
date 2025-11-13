#pragma once

#include <map>

#include <engine/input/key_code.h>
#include <engine/input/key_state.h>
#include <engine/input/mouse_state.h>

class IInput {
public:
    virtual ~IInput() = default;

    /**
    * @brief Updates the state of keyboard and mouse inputs.
    *
    * This function is responsible for querying the current input states, including
    * key presses, key releases, mouse button presses, releases, and mouse movement.
    * It updates the provided `key_states` and `mouse_state` with the latest input
    * information for the current frame.
    */
    virtual void update(std::map<KeyCode, KeyState>& key_states, MouseState& mouse_state);
};
