#include <engine/physics/physics_raycaster.h>

PhysicsRaycaster::PhysicsRaycaster(b2WorldId world_id)
    : world_id_(world_id) {}

b2RayResult PhysicsRaycaster::raycast(const b2Vec2& origin, const b2Vec2& translation) {
    // Implementation...
}