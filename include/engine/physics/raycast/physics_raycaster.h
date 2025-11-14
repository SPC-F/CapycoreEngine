#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <vector>

#include <engine/public/util/point.h>
#include <engine/physics/raycast/collider_ray_result.h>


/**
 * @brief Class responsible for raycasting in the physics world.
 * 
 * This class provides functionality to perform raycasting
 * operations within the Box2D physics simulation.
 */
class PhysicsRaycaster {
public:
    PhysicsRaycaster(b2WorldId world_id);

    /**
     * @brief Closest hit
     * 
     * Meant for simple raycasts where only the closest hit is needed.
     * 
     * @param origin The starting point of the ray.
     * @param translation The direction and length of the ray.
     * @return ColliderRayResult The result of the closest hit.
     */
    ColliderRayResult raycast_closest(const b2Vec2& origin, const b2Vec2& translation);

   /**
    * @brief All hits
    * 
    * Returns all hits along the ray, sorted by distance from the origin.
    * 
    * @param origin The starting point of the ray.
    * @param translation The direction and length of the ray.
    * @param filter The query filter to apply.
    * @return std::vector<ColliderRayResult> A vector of all raycast results.
    */
    std::vector<ColliderRayResult> raycast_all(const b2Vec2& origin, const b2Vec2& translation, b2QueryFilter filter = {});

    /**
     * @brief Filtered hits by category mask
     * 
     * Returns all hits along the ray that match the given category mask.
     * 
     * @param origin The starting point of the ray.
     * @param translation The direction and length of the ray.
     * @param category_mask The category mask to filter results.
     * @return std::vector<ColliderRayResult> A vector of filtered raycast results
     */
    std::vector<ColliderRayResult> raycast_filtered(const b2Vec2& origin, const b2Vec2& translation, uint16_t category_mask);

    /**
     * @brief Raycast segment
     * 
     * Performs a raycast between two points, returning the first hit encountered.
     * 
     * @param start The starting point of the segment.
     * @param end The ending point of the segment.
     * @return ColliderRayResult The result of the segment raycast.
     */
    ColliderRayResult raycast_segment(const b2Vec2& start, const b2Vec2& end);

private:
    b2WorldId world_id_;
};