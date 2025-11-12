#pragma once

#include <map>

#include <engine/input/key_code.h>
#include <engine/input/key_state.h>
#include <engine/input/mouse_state.h>

class IInput {
public:
    virtual ~IInput() = default;

    virtual void update(std::map<KeyCode, KeyState>& key_states, MouseState& mouse_state);
};
