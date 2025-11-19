#include <engine/physics/physics_service.h>

PhysicsService::PhysicsService() 
{
    physics_world_ = std::make_unique<PhysicsWorld>();
    physics_raycaster_ = std::make_unique<PhysicsRaycaster>(physics_world_->world_id());
    physics_creation_factory_ = std::make_unique<PhysicsCreationFactory>(physics_world_->world_id());
}

void PhysicsService::update(float dt, const std::vector<std::reference_wrapper<GameObject>>& objects) 
{
    physics_world_->step(dt);
    physics_world_->check_collision(objects);
}

std::unique_ptr<PhysicsWorld>& PhysicsService::world() noexcept 
{
    return physics_world_;
}

std::unique_ptr<PhysicsRaycaster>& PhysicsService::raycaster() noexcept 
{
    return physics_raycaster_;
}

std::unique_ptr<PhysicsCreationFactory>& PhysicsService::factory() noexcept 
{
    return physics_creation_factory_;
}
