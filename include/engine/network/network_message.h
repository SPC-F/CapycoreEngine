#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

enum class DefaultMessageTypes : uint16_t {
    CONNECT,
    DISCONNECT,
    CLIENT_CONNECT,
    HOST_DISCONNECT,
    CLIENT_DISCONNECT
};

enum class CustomMessageTypes : uint16_t;

struct MessageType
{
    enum class Kind : uint8_t {
        Default,
        Custom
    };

    Kind kind;
    uint16_t value;

    MessageType() = default;

    MessageType(DefaultMessageTypes t)
        : kind{Kind::Default}, value{static_cast<uint16_t>(t)}
    {}

    MessageType(CustomMessageTypes t)
        : kind{Kind::Custom}, value{static_cast<uint16_t>(t)}
    {}

    MessageType(Kind kind, uint16_t t)
        : kind{kind}, value{t}
    {}

    // pack a `MessageType` into a 16-bit integer
    static uint16_t to_raw(const MessageType& type)
    {
        return (type.kind == MessageType::Kind::Custom ? 0x8000 : 0x0000) |
            (type.value & 0x7FFF);
    }

    // parse a `MessageType` from a 16-bit integer
    static MessageType from_raw(uint16_t raw)
    {
        bool isCustom = raw & 0x8000;
        uint16_t value = raw & 0x7FFF;

        return { isCustom ? MessageType::Kind::Custom : MessageType::Kind::Default, value };
    }

    bool operator<(const MessageType& other) const {
        if (kind != other.kind)
            return kind < other.kind;
        return value < other.value;
    }
};

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
Message serialize_message(const T& data, MessageType type)
{
    Message msg;
    msg.header.type = type;
    msg.header.size = sizeof(T);
    msg.payload.resize(sizeof(T));
    std::memcpy(msg.payload.data(), &data, sizeof(T));
    return msg;
}

template <typename T>
T deserialize_message(const Message& message)
{
    T data;
    std::memcpy(&data, message.payload.data(), sizeof(T));
    return data;
}
