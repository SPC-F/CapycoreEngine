#pragma once

#include <cstdint>

constexpr uint16_t default_collision_category = 0x0001;
constexpr uint16_t default_collision_mask = 0xFFFF;

/**
 * @brief Flags for creating physics bodies.
 * 
 * This struct encapsulates various options that can be set
 * when creating physics bodies in the Box2D world.
 */
struct PhysicsCreationFlags
{
    /**
     * @brief Density of the body
     * 
     * Default is 1.0f if not specified.
     */
    bool dynamic{true};

    /**
     * @brief Whether the body is a sensor
     * 
     * Default is false.
     * A sensor does not generate collision responses but can detect overlaps.
     */
    bool sensor{false};

    /**
     * @brief Whether to enable contact events for the body
     * 
     * Default is true.
     * If true, the body will generate contact events on collisions.
     */
    bool enable_contact_events{true};

    /**
     * @brief Whether the body is treated as a bullet
     * 
     * Default is false.
     * A bullet is a fast-moving object that requires continuous collision detection.
     */
    bool is_bullet{false};

    /**
     * @brief Collision category filtering
     *
     * Default is 0x0001.
     * This value defines the category of the body for collision filtering. E.g, 0x0002 for player, 0x0004 for enemies.
     */
    uint16_t category = default_collision_category;

    /**
     * @brief Collision mask filtering
     *
     * Default is 0xFFFF | 0x0001.
     * This value defines which categories this body collides with. E.g, 0x0002 | 0x0004 to collide with players and enemies.
     */
    uint16_t mask = default_collision_mask;
};