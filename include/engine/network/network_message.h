#pragma once

#include <cstdint>
#include <vector>
#include <variant>
#include <cstring>

enum class DefaultMessageTypes : uint16_t {
    CONNECT,
    DISCONNECT
};

enum class CustomMessageTypes : uint16_t;

using MessageType = std::variant<DefaultMessageTypes, CustomMessageTypes>;

struct MessageHeader
{
    MessageType type;
    uint32_t size;
};

struct Message
{
    MessageHeader header;
    std::vector<uint8_t> payload;
};

// +------ Message Structures ------+

struct MsgConnect
{
    char uuid[37];
};

struct MsgDisconnect
{
    char uuid[37];
};

// +------ Serialization/Deserialization ------+
template <typename T>
Message SerializeMessage(const T& data, MessageType type)
{
    Message msg;
    msg.header.type = type;
    msg.header.size = sizeof(T);
    msg.payload.resize(sizeof(T));
    std::memcpy(msg.payload.data(), &data, sizeof(T));
    return msg;
}

template <typename T>
T DeserializeMessage(const Message& message)
{
    T data;
    std::memcpy(&data, message.payload.data(), sizeof(T));
    return data;
}
