#pragma once

#include <engine/public/components/colliders/collider_2d.h>
#include <engine/public/util/point.h>

constexpr float default_circle_collider_radius = 0.5f;

/**
 * @brief A 2D circle collider component that defines a circular collision shape for a GameObject.
 * 
 * This component extends the Collider2D class to provide a circle-shaped collider,
 * enabling circular collision detection and response within the physics engine.
 */
class CircleCollider2D : public Collider2D
{
public:
    CircleCollider2D(
        float friction, 
        float bounciness,
        float radius = default_circle_collider_radius,
        Point offset = {0.0f, 0.0f}
    );
    ~CircleCollider2D() override = default;

    void update(float dt) override;

    void on_serialize() override;
    void on_deserialize() override;

    [[nodiscard]]
    float radius() const noexcept;
    CircleCollider2D& radius(float value) noexcept;

    CircleCollider2D& friction(float value) noexcept override;
    CircleCollider2D& bounciness(float value) noexcept override;

private:
    float radius_;
};