#pragma once

#include <box2d/box2d.h>
#include <memory>

constexpr float default_step = 1.0f / 60.0f;
constexpr int32_t default_velocity_iterations = 6;
constexpr float default_gravity_x = 0.0f;
constexpr float default_gravity_y = 30.0f;
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

    /** @brief Advances the physics simulation by one time step. */
    void step();

    b2WorldId world_id() noexcept;

    [[nodiscard]]
    float time_step() const noexcept;
    float time_step(float timestep) noexcept;

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

private:
    /**
     * @brief Unique identifier for the Box2D world instance.
     *
     * Stored as a smart pointer with a custom deleter that
     * automatically calls `b2DestroyWorld` when the world is destroyed.
     */
    b2WorldId world_id_{};

    /** @brief Fixed time step for physics updates. */
    float time_step_{default_step};

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