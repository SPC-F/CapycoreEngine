#include <engine/public/components/colliders/circle_collider_2d.h>

#include <stdexcept>

#include <engine/public/gameObject.h>

CircleCollider2D::CircleCollider2D(
    float friction, 
    float bounciness, 
    float mass,
    float radius
) : Collider2D(friction, bounciness, mass), radius_(radius) 
{
    if (!parent().has_value()) {
        throw std::runtime_error("CircleCollider2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
        auto& gameobject = gameobject_opt->get();

        if (!gameobject.get_component<Rigidbody2D>().has_value()) {
            throw std::runtime_error("CircleCollider2D requires a Rigidbody2D component on the same GameObject.");
        }

        if (gameobject.get_component<CircleCollider2D>().has_value()) {
            throw std::runtime_error("GameObject cannot have multiple CircleCollider2D components.");
        }
    }
}

void CircleCollider2D::update() 
{
    Collider2D::update();   
}

void CircleCollider2D::on_serialize() 
{}

void CircleCollider2D::on_deserialize() 
{}

float CircleCollider2D::radius() const noexcept 
{
    return radius_;
}

CircleCollider2D& CircleCollider2D::radius(float value) noexcept 
{
    radius_ = value;
    return *this;
}