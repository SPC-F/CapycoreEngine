#include <engine/input/strategy/sdl_input_strategy.h>

#include <SDL3/SDL_events.h>

void SDLInputStrategy::update(std::map<KeyCode, KeyState>& key_states, MouseState& mouse_state)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            key_states[sdl_to_keycode(event.key.key)].current = true;
            break;

        case SDL_EVENT_KEY_UP:
            key_states[sdl_to_keycode(event.key.key)].current = false;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mouse_state.buttons[sdl_to_mouse_button(event.button.button)].current = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            mouse_state.buttons[sdl_to_mouse_button(event.button.button)].current = false;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            mouse_state.position.x = event.motion.x;
            mouse_state.position.y = event.motion.y;
            mouse_state.delta_x += static_cast<float>(event.motion.xrel);
            mouse_state.delta_y += static_cast<float>(event.motion.yrel);
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            mouse_state.wheel.x_delta += static_cast<float>(event.wheel.x);
            mouse_state.wheel.y_delta += static_cast<float>(event.wheel.y);
            mouse_state.wheel.x_scroll += static_cast<float>(event.wheel.x);
            mouse_state.wheel.y_scroll += static_cast<float>(event.wheel.y);

            mouse_state.wheel.last_scroll_direction =
                (event.wheel.y > 0) ? MouseDirection::up :
                (event.wheel.y < 0) ? MouseDirection::down :
                (event.wheel.x > 0) ? MouseDirection::right :
                (event.wheel.x < 0) ? MouseDirection::left :
                                      MouseDirection::none;
            break;

        default:
            break;
        }
    }
}

KeyCode SDLInputStrategy::sdl_to_keycode(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_A:
        return KeyCode::a;
    case SDLK_B:
        return KeyCode::b;
    case SDLK_C:
        return KeyCode::c;
    case SDLK_D:
        return KeyCode::d;
    case SDLK_E:
        return KeyCode::e;
    case SDLK_F:
        return KeyCode::f;
    case SDLK_G:
        return KeyCode::g;
    case SDLK_H:
        return KeyCode::h;
    case SDLK_I:
        return KeyCode::i;
    case SDLK_J:
        return KeyCode::j;
    case SDLK_K:
        return KeyCode::k;
    case SDLK_L:
        return KeyCode::l;
    case SDLK_M:
        return KeyCode::m;
    case SDLK_N:
        return KeyCode::n;
    case SDLK_O:
        return KeyCode::o;
    case SDLK_P:
        return KeyCode::p;
    case SDLK_Q:
        return KeyCode::q;
    case SDLK_R:
        return KeyCode::r;
    case SDLK_S:
        return KeyCode::s;
    case SDLK_T:
        return KeyCode::t;
    case SDLK_U:
        return KeyCode::u;
    case SDLK_V:
        return KeyCode::v;
    case SDLK_W:
        return KeyCode::w;
    case SDLK_X:
        return KeyCode::x;
    case SDLK_Y:
        return KeyCode::y;
    case SDLK_Z:
        return KeyCode::z;

    case SDLK_0:
        return KeyCode::num_0;
    case SDLK_1:
        return KeyCode::num_1;
    case SDLK_2:
        return KeyCode::num_2;
    case SDLK_3:
        return KeyCode::num_3;
    case SDLK_4:
        return KeyCode::num_4;
    case SDLK_5:
        return KeyCode::num_5;
    case SDLK_6:
        return KeyCode::num_6;
    case SDLK_7:
        return KeyCode::num_7;
    case SDLK_8:
        return KeyCode::num_8;
    case SDLK_9:
        return KeyCode::num_9;

    case SDLK_F1:
        return KeyCode::f1;
    case SDLK_F2:
        return KeyCode::f2;
    case SDLK_F3:
        return KeyCode::f3;
    case SDLK_F4:
        return KeyCode::f4;
    case SDLK_F5:
        return KeyCode::f5;
    case SDLK_F6:
        return KeyCode::f6;
    case SDLK_F7:
        return KeyCode::f7;
    case SDLK_F8:
        return KeyCode::f8;
    case SDLK_F9:
        return KeyCode::f9;
    case SDLK_F10:
        return KeyCode::f10;
    case SDLK_F11:
        return KeyCode::f11;
    case SDLK_F12:
        return KeyCode::f12;

    case SDLK_LSHIFT:
        return KeyCode::left_shift;
    case SDLK_RSHIFT:
        return KeyCode::right_shift;
    case SDLK_LCTRL:
        return KeyCode::left_control;
    case SDLK_RCTRL:
        return KeyCode::right_control;
    case SDLK_LALT:
        return KeyCode::left_alt;
    case SDLK_RALT:
        return KeyCode::right_alt;
    case SDLK_LGUI:
        return KeyCode::left_super;
    case SDLK_RGUI:
        return KeyCode::right_super;
    case SDLK_CAPSLOCK:
        return KeyCode::caps_lock;

    case SDLK_ESCAPE:
        return KeyCode::escape;
    case SDLK_TAB:
        return KeyCode::tab;
    case SDLK_RETURN:
        return KeyCode::enter;
    case SDLK_SPACE:
        return KeyCode::space;
    case SDLK_BACKSPACE:
        return KeyCode::backspace;
    case SDLK_INSERT:
        return KeyCode::insert;
    case SDLK_DELETE:
        return KeyCode::delete_key;
    case SDLK_HOME:
        return KeyCode::home;
    case SDLK_END:
        return KeyCode::end;
    case SDLK_PAGEUP:
        return KeyCode::page_up;
    case SDLK_PAGEDOWN:
        return KeyCode::page_down;
    case SDLK_UP:
        return KeyCode::arrow_up;
    case SDLK_DOWN:
        return KeyCode::arrow_down;
    case SDLK_LEFT:
        return KeyCode::arrow_left;
    case SDLK_RIGHT:
        return KeyCode::arrow_right;

    case SDLK_GRAVE:
        return KeyCode::grave_accent;
    case SDLK_MINUS:
        return KeyCode::minus;
    case SDLK_EQUALS:
        return KeyCode::equal;
    case SDLK_LEFTBRACKET:
        return KeyCode::left_bracket;
    case SDLK_RIGHTBRACKET:
        return KeyCode::right_bracket;
    case SDLK_BACKSLASH:
        return KeyCode::backslash;
    case SDLK_APOSTROPHE:
        return KeyCode::apostrophe;
    case SDLK_COMMA:
        return KeyCode::comma;
    case SDLK_PERIOD:
        return KeyCode::period;
    case SDLK_SLASH:
        return KeyCode::slash;

    case SDLK_KP_0:
        return KeyCode::kp_0;
    case SDLK_KP_1:
        return KeyCode::kp_1;
    case SDLK_KP_2:
        return KeyCode::kp_2;
    case SDLK_KP_3:
        return KeyCode::kp_3;
    case SDLK_KP_4:
        return KeyCode::kp_4;
    case SDLK_KP_5:
        return KeyCode::kp_5;
    case SDLK_KP_6:
        return KeyCode::kp_6;
    case SDLK_KP_7:
        return KeyCode::kp_7;
    case SDLK_KP_8:
        return KeyCode::kp_8;
    case SDLK_KP_9:
        return KeyCode::kp_9;
    case SDLK_KP_DECIMAL:
        return KeyCode::kp_decimal;
    case SDLK_KP_DIVIDE:
        return KeyCode::kp_divide;
    case SDLK_KP_MULTIPLY:
        return KeyCode::kp_multiply;
    case SDLK_KP_MINUS:
        return KeyCode::kp_subtract;
    case SDLK_KP_PLUS:
        return KeyCode::kp_add;
    case SDLK_KP_ENTER:
        return KeyCode::kp_enter;
    case SDLK_KP_EQUALS:
        return KeyCode::kp_equal;

    default:
        return KeyCode::unknown;
    }
}

MouseButton SDLInputStrategy::sdl_to_mouse_button(Uint8 button)
{
    switch (button)
    {
        case SDL_BUTTON_LEFT:
            return MouseButton::left;
        case SDL_BUTTON_RIGHT:
            return MouseButton::right;
        case SDL_BUTTON_MIDDLE:
            return MouseButton::middle;

        case SDL_BUTTON_X1:
            return MouseButton::button_4;
        case SDL_BUTTON_X2:
            return MouseButton::button_5;

        default:
            return MouseButton::unknown;
    }
}
