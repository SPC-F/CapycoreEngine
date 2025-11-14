#include <engine/public/components/colliders/circle_collider_2d.h>

#include <stdexcept>

#include <engine/public/gameObject.h>

CircleCollider2D::CircleCollider2D(
    float friction, 
    float bounciness,
    float radius
) : Collider2D(friction, bounciness), radius_(radius) 
{
    if (!parent().has_value()) {
        throw std::runtime_error("CircleCollider2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
        auto& gameobject = gameobject_opt->get();

       auto rigidbody_opt = gameobject.get_component<Rigidbody2D>();
        auto circle_collider_opt = gameobject.get_component<CircleCollider2D>();
        
        if (!rigidbody_opt.has_value()) {
            throw std::runtime_error("CircleCollider2D requires a Rigidbody2D component on the same GameObject.");
        }

        if (circle_collider_opt.has_value()) {
            throw std::runtime_error("GameObject cannot have multiple CircleCollider2D components.");
        }

        auto& rigidbody = rigidbody_opt->get();
        auto body = rigidbody.body();
        rigidbody.body(PhysicsCreationFactory::create_circle_fixture(
            body, 
            radius_,
            Collider2D::creation_flags()
        ));
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

CircleCollider2D& CircleCollider2D::friction(float value) noexcept 
{
    Collider2D::friction(value);

    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_friction(rigidbody.body(), value, ShapeType2D::Circle);

    return *this;
}

CircleCollider2D& CircleCollider2D::bounciness(float value) noexcept 
{
    Collider2D::bounciness(value);

    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_bounciness(rigidbody.body(), value, ShapeType2D::Circle);
    
    return *this;
}