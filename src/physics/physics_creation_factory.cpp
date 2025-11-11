#include <engine/physics/physics_creation_factory.h>

PhysicsCreationFactory::PhysicsCreationFactory(b2WorldId world_id)
    : world_id_(world_id) {}

b2BodyId PhysicsCreationFactory::create_box_body(Point position, float width, float height, PhysicsCreationFlags flags) {
    // Implementation...
    return b2BodyId{};    
}


b2BodyId PhysicsCreationFactory::create_circle_body(Point position, float radius, PhysicsCreationFlags flags) {
    // Implementation...
    return b2BodyId{};
}

void PhysicsCreationFactory::destroy_body(b2BodyId body_id) {
    // Implementation...
}