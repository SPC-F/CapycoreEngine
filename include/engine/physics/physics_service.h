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
     * 
     * @param dt The (deltatime) time step duration for the physics update.
     */
    void update(float dt);

    /** 
     * @brief Accessor for the physics world instance.
     * 
     * @return std::unique_ptr<PhysicsWorld>& Reference to the physics world instance.
     */
    std::unique_ptr<PhysicsWorld>& world() noexcept;

    /**
     * @brief Accessor for the physics raycaster instance.
     * 
     * @return std::unique_ptr<PhysicsRaycaster>& Reference to the physics raycaster instance.
     */
    std::unique_ptr<PhysicsRaycaster>& raycaster() noexcept;

    /**
     * @brief Accessor for the physics creation factory instance.
     * 
     * @return std::unique_ptr<PhysicsCreationFactory>& Reference to the physics creation factory instance.
     */
    std::unique_ptr<PhysicsCreationFactory>& factory() noexcept;

private:
    std::unique_ptr<PhysicsWorld> physics_world_{nullptr};
    std::unique_ptr<PhysicsRaycaster> physics_raycaster_{nullptr};
    std::unique_ptr<PhysicsCreationFactory> physics_creation_factory_{nullptr};
}; 
