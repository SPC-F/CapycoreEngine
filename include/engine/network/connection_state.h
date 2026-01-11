#pragma once
#include <cstdint>

enum class ConnectionState : uint16_t {
  NONE,
  CONNECTED,
  DISCONNECTED,
  CONNECTING,
  DISCONNECTING,
  RECONNECTING  // Unimplemented
};