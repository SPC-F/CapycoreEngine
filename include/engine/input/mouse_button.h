#pragma once

#include <cstdint>

enum class MouseButton : std::uint8_t {
    // Primary buttons
    left, right, middle,

    // Extra buttons
    button_4, button_5,

    // Wheel input
    wheel_up, wheel_down,

    // Fallback
    unknown
};
