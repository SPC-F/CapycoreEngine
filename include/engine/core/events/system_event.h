#pragma once

#include <cstdint>

enum SystemEvent : uint8_t {
  EVENT_QUIT = 0,
  KEY_DOWN = 1,
  KEY_UP = 2,
  MOUSE_BUTTON_DOWN = 3,
  MOUSE_BUTTON_UP = 4,
  MOUSE_MOTION = 5,
  MOUSE_WHEEL = 6
};
