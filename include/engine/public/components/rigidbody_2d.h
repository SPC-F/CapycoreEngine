#pragma once

#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/public/component.h>

/**
 * @brief A 2D rigidbody component that adds physics properties to a GameObject.
 * 
 * This component allows a GameObject to interact with the physics engine,
 * enabling features such as gravity, collisions, and movement based on physical forces.
 */
class Rigidbody2D : public Component
{
public:
    Rigidbody2D(BodyType2D::Type type = BodyType2D::Dynamic, 
                float mass = 1.0f,
                bool use_gravity = true,
                float gravity_scale = 1.0f);
    ~Rigidbody2D() override;

    void update() override;
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
    float gravity_scale() const noexcept;
    Rigidbody2D& gravity_scale(float value) noexcept;

    [[nodiscard]]
    Body2D body() const noexcept;
    void body(const Body2D& value) noexcept;

private:
    Body2D body_ {};
    BodyType2D::Type type_;
    float mass_ {1.0f};
    
    bool use_gravity_ {true};
    float gravity_scale_ {1.0f};

};