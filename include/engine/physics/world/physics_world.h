#pragma once

#include <box2d/box2d.h>
#include <memory>

#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/physics/world/body/body_2d.h>
#include <engine/physics/world/body/shape_type_2d.h>
#include <engine/physics/world/body/body_distance_2d.h>
#include <engine/public/util/vector3.h>
#include <engine/public/util/point.h>

constexpr int32_t default_velocity_iterations = 6;
constexpr float default_gravity_x = 0.0f;
constexpr float default_gravity_y = -30.0f;
constexpr float default_pixel_to_meter_ratio = 16.0f;

/**
 * @brief Represents the physics world using Box2D.
 *
 * This class encapsulates a Box2D world instance and provides
 * settings such as gravity, time step, and conversion ratios.
 */
class PhysicsWorld {
public:
    /** @brief Constructs a new physics world. */
    PhysicsWorld();

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;
    PhysicsWorld(PhysicsWorld&&) = delete;
    PhysicsWorld& operator=(PhysicsWorld&&) = delete;
    
    ~PhysicsWorld();

    /**
     * @brief Steps the physics simulation forward by the fixed time step.
     * 
     * This method advances the physics world by the configured time step,
     * applying velocity and position updates to all bodies in the world.
     * 
     * @param dt The (deltatime) time step duration for the physics update.
     */
    void step(float dt);
    
    /**
     * @brief Checks for collisions in the physics world.
     * 
     * This method processes collision detection and resolution
     * for the current state of the physics world. Its based on
     * Box2D's internal collision handling and objects
     */
    void check_collision(const std::vector<std::reference_wrapper<GameObject>>& objects);

    /**
     * @brief Calculate the distance between two bodies
     * 
     * @param a The first body's transform.
     * @param b The second body's transform.
     * @return BodyDistance2D The distance information between the two bodies.
     */
    static BodyDistance2D distance(const Body2DTransform& a, const Body2DTransform& b);

    /**
     * @brief Calculate the distance between two bodies using their fixtures
     * 
     * Fixture means the actual shapes attached to the bodies. So for instance a 
     * box body with multiple box fixtures will calculate the distance based on the closest fixture.
     * 
     * @param a The first body's transform.
     * @param b The second body's transform.
     * @return BodyDistance2D The distance information between the two bodies' fixtures.
     */
    static BodyDistance2D fixture_distance(const Body2DTransform& a, const Body2DTransform& b);

    [[nodiscard]]
    b2WorldId world_id() const noexcept;

    [[nodiscard]]
    int32_t velocity_iterations() const noexcept;
    int32_t velocity_iterations(int32_t iterations) noexcept;

    [[nodiscard]]
    float gravity_x() const noexcept;
    float gravity_x(float gx) noexcept;

    [[nodiscard]]
    float gravity_y() const noexcept;
    float gravity_y(float gy) noexcept;

    [[nodiscard]]
    float pixel_to_meter_ratio() const noexcept;
    float pixel_to_meter_ratio(float ratio) noexcept;

    [[nodiscard]]
    bool world_exists() const noexcept;

private:
    /**
     * @brief Unique identifier for the Box2D world instance.
     *
     * Stored as a simple value as Box2D does not allow copying or moving
     * of its world instances. Meaning a pointer or reference cannot be used here without more complex management.
     */
    b2WorldId world_id_{};

    /** @brief Number of velocity iterations for the Box2D solver. */
    int32_t velocity_iterations_{default_velocity_iterations};

    /** @brief X component of the gravity vector. */
    float gravity_x_{default_gravity_x};

    /** @brief Y component of the gravity vector. */
    float gravity_y_{default_gravity_y};

    /** @brief Conversion ratio from pixels to meters (e.g., 16 px = 1 m). */
    float pixel_to_meter_ratio_{default_pixel_to_meter_ratio};

    /** @brief Flag to enable or disable debug drawing of physics objects. */
    bool debug_draw_enabled_{false};
};