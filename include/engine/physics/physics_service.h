#pragma once

#include <box2d/box2d.h>
#include <memory>

#include <engine/core/iEngineService.h>
#include <engine/physics/physics_world.h>
#include <engine/physics/physics_raycaster.h>
#include <engine/physics/physics_creation_factory.h>
#include <engine/physics/physics_math.h>

/**
 * @brief Service responsible for managing the physics world.
 * 
 * This service encapsulates the physics world and provides
 * functionality to update and interact with the physics simulation.
 */
class PhysicsService : public IEngineService
{
public:
    PhysicsService();

    /** 
     * @brief Updates the physics world by stepping the simulation.
     * 
     * This method advances the physics simulation by one time step,
     * applying all physics calculations and updating object states.
     */
    void update();

private:
    std::unique_ptr<PhysicsWorld> physics_world_{nullptr};
    std::unique_ptr<PhysicsRaycaster> physics_raycaster_{nullptr};
    std::unique_ptr<PhysicsCreationFactory> physics_creation_factory_{nullptr};
}; 
