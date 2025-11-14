#pragma once

#include <box2d/box2d.h>
#include <vector>

/**
 * @brief Structure representing a 2D physics body.
 * 
 * This struct encapsulates the Box2D body identifier
 * along with its associated shapes.
 */
struct Body2D {
    b2BodyId id {};
    std::vector<b2ShapeId> shapes;
};