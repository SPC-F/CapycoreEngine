#pragma once

enum class ConnectionState : uint16_t {
    NONE,
    CONNECTED,
    DISCONNECTED,
    CONNECTING,
    DISCONNECTING,
    RECONNECTING // Unimplemented
};