#include <engine/physics/physics_service.h>

PhysicsService::PhysicsService() {
    physics_world_ = std::make_unique<PhysicsWorld>();
    physics_raycaster_ = std::make_unique<PhysicsRaycaster>(physics_world_->world_id());
    physics_creation_factory_ = std::make_unique<PhysicsCreationFactory>(physics_world_->world_id());
}

void PhysicsService::update() {
    physics_world_->step();
}
