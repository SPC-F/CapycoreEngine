#include <engine/physics/physics_creation_factory.h>

PhysicsCreationFactory::PhysicsCreationFactory(b2WorldId world_id)
    : world_id_(world_id) {}

std::unique_ptr<b2BodyId, decltype(&b2DestroyBody)> 
PhysicsCreationFactory::create_box_body(Point position, float width, float height, PhysicsCreationFlags flags) {
    // Implementation...
}

std::unique_ptr<b2BodyId, decltype(&b2DestroyBody)> 
PhysicsCreationFactory::create_circle_body(Point position, float radius, PhysicsCreationFlags flags) {
    // Implementation...
}