#pragma once

#include <box2d/box2d.h>
#include <engine/public/util/point.h>

/**
 * @brief Structure representing the result of a raycast in the physics world.
 * 
 * This struct encapsulates the distance, hit point, normal vector,
 * and validity of a raycast operation.
 */
struct ColliderRayResult
{
    b2ShapeId shape_id {};
    float fraction {0.0f};
    bool is_valid {false};

    float distance {0.0f};
    Point point {0.0f, 0.0f};
    Point normal {0.0f, 0.0f};
};