#pragma once

#include <engine/public/component.h>
#include <engine/physics/physics_creation_factory.h>

/**
 * @brief A 2D rigidbody component that adds physics properties to a GameObject.
 * 
 * This component allows a GameObject to interact with the physics engine,
 * enabling features such as gravity, collisions, and movement based on physical forces.
 */
class Rigidbody2D : public Component
{
public:
    Rigidbody2D();
    ~Rigidbody2D() override;

    void update() override;
    void on_attach() override;
    void on_detach() override;
    void on_serialize() override;
    void on_deserialize() override;

    [[nodiscard]] 
    BodyType2D::Type type() const noexcept;
    Rigidbody2D& type(BodyType2D::Type value) noexcept;

    [[nodiscard]] 
    float mass() const noexcept;
    Rigidbody2D& mass(float value) noexcept;

    [[nodiscard]] 
    bool use_gravity() const noexcept;
    Rigidbody2D& use_gravity(bool value) noexcept;

    [[nodiscard]] 
    float gravity_scale_x() const noexcept;
    Rigidbody2D& gravity_scale_x(float value) noexcept;

    [[nodiscard]] 
    float gravity_scale_y() const noexcept;
    Rigidbody2D& gravity_scale_y(float value) noexcept;

private:
    Body2D body_ {};
    BodyType2D::Type type_ {BodyType2D::Dynamic};
    float mass_ {1.0f};
    
    bool use_gravity_ {true};
    float gravity_scale_x_ {1.0f};
    float gravity_scale_y_ {1.0f};

};