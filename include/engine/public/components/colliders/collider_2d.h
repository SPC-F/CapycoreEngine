#pragma once

#include <functional>
#include <vector>

#include <engine/physics/creation/physics_creation_flags.h>
#include <engine/physics/raycast/physics_raycaster.h>
#include <engine/physics/world/physics_world.h>
#include <engine/public/component.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/util/point.h>

constexpr float default_friction_2d = 0.5f;
constexpr float default_bounciness_2d = 0.5f;

/**
 * @brief A 2D collider base that defines the shape and physical properties of a GameObject for collision detection.
 * @note This is an abstract base class and should be inherited by specific collider shapes like CircleCollider2D, BoxCollider2D, etc.
 * 
 * This component allows a GameObject to participate in collision detection and response within the physics engine.
 * It provides methods for handling collision events and querying collision information.
 */
class Collider2D : public Component
{
public:
    Collider2D(
        float friction = default_friction_2d, 
        float bounciness = default_bounciness_2d
    );
    ~Collider2D() override = default;

    void update(float dt) override;

    /**
     * @brief Called when another collider enters this collider's trigger area.
     * @note This method can be overridden to implement custom behavior on trigger enter events.
     * @param other The other collider that entered the trigger.
     */
    virtual void on_trigger_enter(Collider2D& other);
    /**
     * @brief Called when another collider exits this collider's trigger area.
     * @note This method can be overridden to implement custom behavior on trigger exit events.
     * @param other The other collider that exited the trigger.
     */
    virtual void on_trigger_exit(Collider2D& other);

    /**
     * @brief Called when another collider exits this collider's trigger area.
     * @note This method can be overridden to implement custom behavior on trigger exit events.
     * @param other The other collider that exited the trigger.
     */
    virtual void on_collision_enter(Collider2D& other);
    /**
     * @brief Called when another collider exits this collider's trigger area.
     * @note This method can be overridden to implement custom behavior on trigger exit events.
     * @param other The other collider that exited the trigger.
     */
    virtual void on_collision_exit(Collider2D& other);
    
    /**
     * @brief Calculate the distance to another collider.
     * 
     * @param other The other collider to measure distance to.
     * @param use_fixture Whether to use fixture shapes for distance calculation.
     * @return BodyDistance2D The distance information between the two colliders.
     */
    BodyDistance2D distance(Collider2D& other, bool use_fixture = false);

    /**
     * @brief Perform a raycast from this collider in a specified direction.
     * 
     * @param direction The direction of the raycast.
     * @param max_distance The maximum distance the ray should check for collisions.
     * @return ColliderRayResult The result of the raycast.
     */
    ColliderRayResult raycast(const Point& direction, float max_distance);

    [[nodiscard]]
    std::reference_wrapper<Rigidbody2D> get_rigidbody();

    [[nodiscard]]
    float friction() const noexcept;
    virtual Collider2D& friction(float value) noexcept;

    [[nodiscard]]
    float bounciness() const noexcept;
    virtual Collider2D& bounciness(float value) noexcept;

    PhysicsCreationFlags& creation_flags() noexcept;
    Collider2D& creation_flags(PhysicsCreationFlags value) noexcept;

    
private:
    PhysicsCreationFlags creation_flags_ {};
    float friction_;
    float bounciness_;

    std::vector<std::function<void(Collider2D&, Collider2D&)>> on_trigger_enter_actions_;
    std::vector<std::function<void(Collider2D&, Collider2D&)>> on_trigger_exit_actions_;
    std::vector<std::function<void(Collider2D&, Collider2D&)>> on_collision_enter_actions_;
    std::vector<std::function<void(Collider2D&, Collider2D&)>> on_collision_exit_actions_;

}; 