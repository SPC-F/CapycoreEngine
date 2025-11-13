#pragma once

#include <map>

#include <engine/input/i_input.h>
#include <engine/input/strategy/sdl_input_strategy.h>

#include <SDL3/SDL_keycode.h>

class SDLInputStrategy : public IInput {
public:
    void update(std::map<KeyCode, KeyState>& key_states, MouseState& mouse_state) override;

private:
    /**
     * @brief Converts an SDL_Keycode to a KeyCode enum.
     *
     * This method maps an SDL keycode to a platform-independent KeyCode, enabling
     * translation between SDL input events and the internal key representation.
     */
    KeyCode sdl_to_keycode(SDL_Keycode key);

    /**
     * @brief Converts an SDL mouse button ID to a MouseButton enum.
     *
     * This method maps an SDL mouse button (e.g., SDL_BUTTON_LEFT) to a platform-independent
     * MouseButton enum value, enabling consistent mouse button handling across different platforms.
     */
    MouseButton sdl_to_mouse_button(Uint8 button);
};
