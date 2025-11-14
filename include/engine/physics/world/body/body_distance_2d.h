#pragma once

#include <engine/public/util/point.h>

/**
 * @brief Structure representing the distance information between two colliders.
 * 
 * This struct encapsulates the distance, normal vector, and contact points
 * between two physics bodies in the world.
 */
struct BodyDistance2D 
{
    float distance {0.0f};
    Point normal {0.0f, 0.0f};
    Point point_a {0.0f, 0.0f};
    Point point_b {0.0f, 0.0f};
};