#include <utility>

#include <engine/input/mouse_button.h>
#include <engine/input/mouse_state.h>

void MouseButtonState::reset()
{
    current = false;
    previous = false;
}

void MouseWheelState::reset()
{
    x_scroll = 0.0f;
    y_scroll = 0.0f;
    x_delta = 0.0f;
    y_delta = 0.0f;
    last_scroll_direction = MouseDirection::none;
}

void MouseState::reset()
{
    position = Point(0, 0);
    delta_x = 0.0f;
    delta_y = 0.0f;

    for (std::pair<const MouseButton, MouseButtonState>& button : buttons)
        button.second.reset();

    wheel.reset();
}
