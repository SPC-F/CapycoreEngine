#pragma once

#include <engine/public/component.h>
#include <engine/public/util/point.h>

struct ColliderDistance2D
{
    float distance {0.0f};
    float is_overlapped {false};
    float is_valid {false};
    Point point_a {0.0f, 0.0f};
    Point point_b {0.0f, 0.0f};
};

struct ColliderRayResult2D
{
    float distance {0.0f};
    Point point {0.0f, 0.0f};
    Point normal {0.0f, 0.0f};
    bool is_valid {false};
};

/**
 * @brief A 2D collider base that defines the shape and physical properties of a GameObject for collision detection.
 * 
 * This component allows a GameObject to participate in collision detection and response within the physics engine.
 * It provides methods for handling collision events and querying collision information.
 */
class Collider2D : public Component
{
    Collider2D() = default;
    virtual ~Collider2D() override = default;

    virtual void on_trigger_enter(Collider2D& other) = 0;
    virtual void on_trigger_exit(Collider2D& other) = 0;

    virtual void on_collision_enter(Collider2D& other) = 0;
    virtual void on_collision_exit(Collider2D& other) = 0;

    virtual ColliderDistance2D distance(Collider2D& other) = 0;
    virtual ColliderRayResult2D raycast(const Point& origin, const Point& direction, float max_distance) = 0;

    [[nodiscard]]
    float friction() const noexcept;
    Collider2D& friction(float value) noexcept;

    [[nodiscard]]
    float bounciness() const noexcept;
    Collider2D& bounciness(float value) noexcept;

    [[nodiscard]]
    float mass() const noexcept;
    Collider2D& mass(float value) noexcept;
private:
    float friction_ {0.5f};
    float bounciness_ {0.5f};
    float mass_ {1.0f};
}; 