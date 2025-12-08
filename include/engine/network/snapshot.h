#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/scene.h>
#include <engine/network/network_message.h>

#include <vector>
#include <string>
#include <cstdint>

/**
 * @namespace snapshot
 * @brief Snapshot serialization/deserialization utilities for networked state synchronization.
 *
 * These helpers provide a clean API for packing GameObject state into network messages
 * and unpacking them on remote clients. The snapshot payloads are binary and compact:
 *   uint16_t count
 *   for each object:
 *     char uuid[37]
 *     float x, y, z (position)
 *
 * The engine exposes creators for full snapshots and delta snapshots and corresponding
 * apply helpers for clients.
 */
namespace snapshot {

/**
 * @brief Builds a full snapshot message containing all networked GameObjects in a scene.
 * @param scene The scene to snapshot.
 * @param messageType The message type to assign (usually SNAPSHOT_FULL).
 * @return A Message ready to send, containing serialized GameObject state.
 */
Message create_full_snapshot(const Scene& scene, DefaultMessageTypes messageType);

/**
 * @brief Builds a delta snapshot message containing only changed (dirty) networked objects.
 *
 * Implementations should mark objects dirty (via their NetworkIdentity) when their
 * relevant state changes. This function clears the dirty flag for included objects.
 *
 * @param scene The scene to snapshot.
 * @param messageType The message type to assign for delta snapshots.
 * @return A Message containing only changed objects since the last snapshot.
 */
Message create_delta_snapshot(const Scene& scene, DefaultMessageTypes messageType);

/**
 * @brief Applies a full snapshot to a scene, creating/updating GameObjects as needed.
 *
 * For each object in the snapshot:
 * - If an object with matching NetworkIdentity UUID exists, updates its position.
 * - Otherwise, creates a new GameObject, attaches a NetworkIdentity, and applies the position.
 *
 * @param scene Target scene to apply snapshot to.
 * @param msg The snapshot message to deserialize.
 * @param spriteTexture Name of the sprite texture to use for newly created objects.
 */
void apply_full_snapshot(Scene& scene, const Message& msg);

/**
 * @brief Applies a delta snapshot to a scene, updating only the listed objects.
 *
 * This will update transforms for existing networked objects and create missing ones
 * if necessary.
 */
void apply_delta_snapshot(Scene& scene, const Message& msg);

} // namespace snapshot
