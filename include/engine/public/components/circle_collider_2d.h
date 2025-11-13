#pragma once

#include <engine/public/components/collider_2d.h>

/**
 * @brief A 2D circle collider component that defines a circular collision shape for a GameObject.
 * 
 * This component extends the Collider2D class to provide a circle-shaped collider,
 * enabling circular collision detection and response within the physics engine.
 */
class CircleCollider2D : public Collider2D
{
    CircleCollider2D();
    ~CircleCollider2D() override;

    void on_trigger_enter(Collider2D& other) override;
    void on_trigger_exit(Collider2D& other) override;

    void on_collision_enter(Collider2D& other) override;
    void on_collision_exit(Collider2D& other) override;

    ColliderDistance2D distance(Collider2D& other) override;
    ColliderRayResult2D raycast(const Point& origin, const Point& direction, float max_distance) override;

    void update() override;

    void on_attach() override;
    void on_detach() override;

    void on_serialize() override;
    void on_deserialize() override;

    [[nodiscard]]
    float radius() const noexcept;
    CircleCollider2D& radius(float value) noexcept;

private:
    float radius_ {0.5f};
};