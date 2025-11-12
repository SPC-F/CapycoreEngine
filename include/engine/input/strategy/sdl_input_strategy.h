#pragma once

#include <map>

#include <engine/input/i_input.h>
#include <engine/input/strategy/sdl_input_strategy.h>

#include <SDL3/SDL_keycode.h>

class SDLInputStrategy : public IInput {
public:
    void update(std::map<KeyCode, KeyState>& key_states, MouseState& mouse_state) override;

private:
    KeyCode sdl_to_keycode(SDL_Keycode key);
    MouseButton sdl_to_mouse_button(Uint8 button);
};
