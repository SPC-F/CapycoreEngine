#pragma once

#include <engine/public/components/collider_2d.h>

/**
 * @brief A 2D box collider component that defines a rectangular collision shape for a GameObject.
 * 
 * This component extends the Collider2D class to provide a box-shaped collider,
 * enabling rectangular collision detection and response within the physics engine.
 */
class BoxCollider2D : public Collider2D
{
    BoxCollider2D();
    ~BoxCollider2D() override;

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
    float width() const noexcept;
    BoxCollider2D& width(float value) noexcept;

    [[nodiscard]]
    float height() const noexcept;
    BoxCollider2D& height(float value) noexcept;

private:
    float width_ {1.0f};
    float height_ {1.0f};
};