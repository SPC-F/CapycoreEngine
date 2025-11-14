#pragma once

#include <engine/physics/world/body_2d.h>
#include <engine/public/util/vector3.h>

/**
 * @brief Structure representing the transform of a physics body.
 * 
 * This struct encapsulates the position and rotation of a body
 * identified by its b2BodyId within the physics world.
 */
struct Body2DTransform
{
    Body2D body;
    Vector3 position;
    /** @brief rotation in radians */
    float rotation {0.0f};
};