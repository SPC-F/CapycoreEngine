#pragma once
#include <cstdint>

enum RendererFlags : std::uint8_t {
    Fullscreen = 1 << 0,
    Borderless = 1 << 1,
    Resizable = 1 << 2
};