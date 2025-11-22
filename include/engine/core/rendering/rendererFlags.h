#pragma once
#include <cstdint>

enum RendererFlags : uint8_t {
    None = 0,
    Fullscreen = 1 << 0,
    Borderless = 1 << 1,
    Resizable = 1 << 2
};