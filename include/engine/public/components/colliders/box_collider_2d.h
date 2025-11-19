#pragma once

#include <engine/public/components/colliders/collider_2d.h>
#include <engine/public/util/point.h>

constexpr float default_box_collider_width = 1.0f;
constexpr float default_box_collider_height = 1.0f;

/**
 * @brief A 2D box collider component that defines a rectangular collision shape for a GameObject.
 * 
 * This component extends the Collider2D class to provide a box-shaped collider,
 * enabling rectangular collision detection and response within the physics engine.
 */
class BoxCollider2D : public Collider2D
{
public:
    BoxCollider2D(
        float friction, 
        float bounciness,
        float width = default_box_collider_width,
        float height = default_box_collider_height,
        Point offset = {0.0f, 0.0f}
    );
    ~BoxCollider2D() override = default;

    void update(float dt) override;

    void on_serialize() override;
    void on_deserialize() override;

    [[nodiscard]]
    float width() const noexcept;
    BoxCollider2D& width(float value) noexcept;

    [[nodiscard]]
    float height() const noexcept;
    BoxCollider2D& height(float value) noexcept;

    BoxCollider2D& friction(float value) noexcept override;
    BoxCollider2D& bounciness(float value) noexcept override;

private:
    float width_;
    float height_;
};