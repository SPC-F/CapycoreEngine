#include <engine/public/components/colliders/box_collider_2d.h>

#include <stdexcept>

#include <engine/public/gameObject.h>

BoxCollider2D::BoxCollider2D(
    float friction, 
    float bounciness, 
    float mass,
    float width,
    float height
) : Collider2D(friction, bounciness, mass), width_(width), height_(height) 
{
    if (!parent().has_value()) {
        throw std::runtime_error("BoxCollider2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
        auto& gameobject = gameobject_opt->get();

        if (!gameobject.get_component<Rigidbody2D>().has_value()) {
            throw std::runtime_error("BoxCollider2D requires a Rigidbody2D component on the same GameObject.");
        }

        if (gameobject.get_component<BoxCollider2D>().has_value()) {
            throw std::runtime_error("GameObject cannot have multiple BoxCollider2D components.");
        }
    }
}

void BoxCollider2D::update() 
{
    Collider2D::update();   
}

void BoxCollider2D::on_serialize() 
{}

void BoxCollider2D::on_deserialize() 
{}

float BoxCollider2D::width() const noexcept 
{
    return width_;
}

BoxCollider2D& BoxCollider2D::width(float value) noexcept 
{
    width_ = value;
    return *this;
}

float BoxCollider2D::height() const noexcept 
{
    return height_;
}

BoxCollider2D& BoxCollider2D::height(float value) noexcept 
{
    height_ = value;
    return *this;
}