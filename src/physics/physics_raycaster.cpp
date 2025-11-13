#include <engine/physics/physics_raycaster.h>
#include <algorithm>

constexpr uint64_t all_categories = 0xFFFF;
constexpr float raycast_success_fraction = 1.0f;

PhysicsRaycaster::PhysicsRaycaster(b2WorldId world_id)
    : world_id_(world_id) {}

b2RayResult PhysicsRaycaster::raycast_closest(const b2Vec2& origin, const b2Vec2& translation) {
    b2QueryFilter filter{};
    filter.categoryBits = all_categories;
    filter.maskBits = all_categories;

    return b2World_CastRayClosest(world_id_, origin, translation, filter);
}

std::vector<b2RayResult> PhysicsRaycaster::raycast_all(const b2Vec2& origin, const b2Vec2& translation, b2QueryFilter filter) {
    std::vector<b2RayResult> results;
    
    auto raycast_callback = [](b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context) -> float 
    {
        auto* results = static_cast<std::vector<b2RayResult>*>(context);

        b2RayResult result{};
        result.shapeId = shape_id;
        result.point = point;
        result.normal = normal;
        result.fraction = fraction;

        results->push_back(result);
        return raycast_success_fraction;
    };

    b2World_CastRay(world_id_, origin, translation, filter,
                    static_cast<b2CastResultFcn*>(raycast_callback),
                    &results);

    std::sort(results.begin(), results.end(),
              [](const b2RayResult& a, const b2RayResult& b) {
                  return a.fraction < b.fraction;
              });

    return results;
}

std::vector<b2RayResult> PhysicsRaycaster::raycast_filtered(const b2Vec2& origin, const b2Vec2& translation, uint16_t category_mask) {
    b2QueryFilter filter;
    filter.categoryBits = all_categories;
    filter.maskBits = category_mask;

    return raycast_all(origin, translation, filter);
}

b2RayResult PhysicsRaycaster::raycast_segment(const b2Vec2& start, const b2Vec2& end) {
    b2Vec2 translation = end - start;
    
    b2QueryFilter filter;
    filter.categoryBits = all_categories;
    filter.maskBits = all_categories;
    
    return b2World_CastRayClosest(world_id_, start, translation, filter);
}