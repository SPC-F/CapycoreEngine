#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

/**
 * @enum DefaultMessageTypes
 * @brief Built-in protocol message types used for core connection events.
 *
 * These message types represent the minimal set of messages required
 * for establishing and maintaining multiplayer sessions.
 */
enum class DefaultMessageTypes : uint16_t {
    CONNECT,
    DISCONNECT,
    CLIENT_CONNECT,
    HOST_DISCONNECT,
    CLIENT_DISCONNECT,
    SNAPSHOT_FULL,
    SNAPSHOT_DELTA
};

/**
 * @enum CustomMessageTypes
 * @brief User-defined message types. Must be implemented by the engine user.
 *
 * Custom types allow the application layer to define its own networked events.
 */
enum class CustomMessageTypes : uint16_t;

/**
 * @struct MessageType
 * @brief Represents either a default or custom message identifier.
 *
 * MessageType encodes both the message category (Default/Custom)
 * and the underlying 15-bit value. When serialized, the high bit is used
 * to represent whether the type is custom or default.
 */
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

    /**
     * @brief Packs a MessageType into a 16-bit integer.
     *
     * The highest bit indicates whether the type is custom.
     */
    static uint16_t to_raw(const MessageType& type)
    {
        return (type.kind == MessageType::Kind::Custom ? 0x8000 : 0x0000) |
            (type.value & 0x7FFF);
    }

    /**
     * @brief Restores a MessageType from a raw 16-bit value.
     *
     * Top bit → Kind::Custom.
     * Lowest 15 bits → type value.
     */
    static MessageType from_raw(uint16_t raw)
    {
        bool isCustom = raw & 0x8000;
        uint16_t value = raw & 0x7FFF;

        return { isCustom ? MessageType::Kind::Custom : MessageType::Kind::Default, value };
    }

    /**
     * @brief Strict ordering operator for use in associative containers.
     */
    bool operator<(const MessageType& other) const {
        if (kind != other.kind)
            return kind < other.kind;
        return value < other.value;
    }
};

/**
 * @struct MessageHeader
 * @brief Metadata describing the type and payload size of a message.
 */
struct MessageHeader
{
    MessageType type;
    uint32_t size;
};

/**
 * @struct Message
 * @brief Represents a complete network message including type and payload.
 *
 * The payload is a raw byte buffer whose structure is determined by the message type.
 */
struct Message
{
    MessageHeader header;
    std::vector<uint8_t> payload;
};

struct MsgConnect
{
    char uuid[37];
};

struct MsgDisconnect
{
    char uuid[37];
};

/**
 * @brief Serializes a plain-old-data structure into a Message.
 *
 * @tparam T POD type to serialize (must be trivially copyable).
 * @param data The structure to serialize.
 * @param type The associated MessageType.
 * @return A fully constructed Message ready for transmission.
 */
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

/**
 * @brief Deserializes a Message payload into a POD structure.
 *
 * @tparam T Expected message structure type.
 * @param message The incoming message containing serialized bytes.
 * @return A reconstructed instance of type T.
 */
template <typename T>
T deserialize_message(const Message& message)
{
    T data;
    std::memcpy(&data, message.payload.data(), sizeof(T));
    return data;
}
