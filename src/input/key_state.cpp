#include "engine/input/key_state.h"

void KeyState::reset()
{
    current = false;
    previous = false;
}
