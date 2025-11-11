#pragma once

#include <box2d/box2d.h>
#include <memory>

/**
 * @brief Class responsible for raycasting in the physics world.
 * 
 * This class provides functionality to perform raycasting
 * operations within the Box2D physics simulation.
 */
class PhysicsRaycaster {
public:
    PhysicsRaycaster(b2WorldId world_id);

    /** @brief Perform a raycast from the origin in the direction of the translation vector */
    b2RayResult raycast(const b2Vec2& origin, const b2Vec2& translation);

private:
    b2WorldId world_id_;
};