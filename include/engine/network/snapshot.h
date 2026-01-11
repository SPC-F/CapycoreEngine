#pragma once

#include <engine/network/network_message.h>
#include <engine/public/gameObject.h>
#include <engine/public/scene.h>

#include <cstdint>
#include <string>
#include <vector>

/**
 * @namespace snapshot
 * @brief Snapshot serialization/deserialization utilities for networked state
 * synchronization.
 *
 * These helpers provide a clean API for packing GameObject state into network
 * messages and unpacking them on remote clients. The snapshot payloads are
 * binary and compact: uint16_t count for each object: char uuid[37] float x, y,
 * z (position)
 *
 * The engine exposes creators for full snapshots and delta snapshots and
 * corresponding apply helpers for clients.
 */
namespace snapshot {

// Serialization helpers
/**
 * @brief Writes a length-prefixed string to a binary buffer.
 *
 * Encodes the string length as a uint16_t followed by the string data.
 * Used for compact serialization of variable-length strings.
 *
 * @param out Output buffer to write to.
 * @param str String to serialize.
 */
void write_string(std::vector<uint8_t>& out, const std::string& str);

/**
 * @brief Reads a length-prefixed string from a binary buffer.
 *
 * Decodes a uint16_t length prefix and reads the corresponding string data.
 * Advances the offset past the read data.
 *
 * @param data Input buffer to read from.
 * @param offset Current read position; updated to point past the string.
 * @param out Output string to store the deserialized value.
 * @return true if read succeeded, false if buffer bounds exceeded.
 */
bool read_string(const std::vector<uint8_t>& data, size_t& offset,
                 std::string& out);

/**
 * @brief Writes raw bytes to a binary buffer.
 *
 * Appends the specified data to the buffer without any encoding.
 * Used for serializing fixed-size types (floats, ints, etc.).
 *
 * @param out Output buffer to write to.
 * @param data Pointer to data to serialize.
 * @param size Number of bytes to write.
 */
void write_bytes(std::vector<uint8_t>& out, const void* data, size_t size);

/**
 * @brief Reads raw bytes from a binary buffer.
 *
 * Copies the specified number of bytes from the buffer.
 * Advances the offset past the read data.
 *
 * @param data Input buffer to read from.
 * @param offset Current read position; updated to point past the bytes.
 * @param out Output buffer to store the deserialized data.
 * @param size Number of bytes to read.
 * @return true if read succeeded, false if buffer bounds exceeded.
 */
bool read_bytes(const std::vector<uint8_t>& data, size_t& offset, void* out,
                size_t size);

/**
 * @brief Builds a full snapshot message containing all networked GameObjects in
 * a scene.
 * @param scene The scene to snapshot.
 * @param messageType The message type to assign (usually SNAPSHOT_FULL).
 * @return A Message ready to send, containing serialized GameObject state.
 */
Message create_full_snapshot(const Scene& scene,
                             DefaultMessageTypes messageType);

/**
 * @brief Builds a delta snapshot message containing only changed (dirty)
 * networked objects.
 *
 * Implementations should mark objects dirty (via their NetworkIdentity) when
 * their relevant state changes. This function clears the dirty flag for
 * included objects.
 *
 * @param scene The scene to snapshot.
 * @param messageType The message type to assign for delta snapshots.
 * @return A Message containing only changed objects since the last snapshot.
 */
Message create_delta_snapshot(const Scene& scene,
                              DefaultMessageTypes messageType);

/**
 * @brief Applies a full snapshot to a scene, creating/updating GameObjects as
 * needed.
 *
 * For each object in the snapshot:
 * - If an object with matching NetworkIdentity UUID exists, updates its
 * position.
 * - Otherwise, creates a new GameObject, attaches a NetworkIdentity, and
 * applies the position.
 *
 * @param scene Target scene to apply snapshot to.
 * @param msg The snapshot message to deserialize.
 * @param spriteTexture Name of the sprite texture to use for newly created
 * objects.
 */
void apply_full_snapshot(Scene& scene, const Message& msg);

/**
 * @brief Applies a delta snapshot to a scene, updating only the listed objects.
 *
 * This will update transforms for existing networked objects and create missing
 * ones if necessary.
 */
void apply_delta_snapshot(Scene& scene, const Message& msg);

}  // namespace snapshot
