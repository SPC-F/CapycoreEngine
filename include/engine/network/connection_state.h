#pragma once

enum class ConnectionState {
    NONE,
    CONNECTED,
    DISCONNECTED,
    CONNECTING,
    DISCONNECTING,
    RECONNECTING // Unimplemented
};