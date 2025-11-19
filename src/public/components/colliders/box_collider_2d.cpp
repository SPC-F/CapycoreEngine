#include <engine/public/components/colliders/box_collider_2d.h>

#include <stdexcept>

#include <engine/public/gameObject.h>

BoxCollider2D::BoxCollider2D(
    float friction, 
    float bounciness,
    float width,
    float height,
    Point offset
) : Collider2D(friction, bounciness, offset), width_(width), height_(height) 
{
    auto on_awake = [this, offset](Component& comp) {
        if (!parent().has_value()) {
            throw std::runtime_error("BoxCollider2D has no parent GameObject.");
        }

        if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
            auto& gameobject = gameobject_opt->get();
            
            auto rigidbody_opt = gameobject.get_component<Rigidbody2D>();
            auto box_collider_opt = gameobject.get_component<BoxCollider2D>();

            if (!rigidbody_opt.has_value()) {
                throw std::runtime_error("BoxCollider2D requires a Rigidbody2D component on the same GameObject.");
            }

            if (box_collider_opt.has_value()) {
                throw std::runtime_error("GameObject cannot have multiple BoxCollider2D components.");
            }

            auto& rigidbody = rigidbody_opt->get();
            auto body = rigidbody.body();
            rigidbody.body(PhysicsCreationFactory::create_box_fixture(
                body, 
                offset,
                width_, 
                height_,
                Collider2D::creation_flags()
            ));        
        }
    };

    add_on_attach(on_awake);
}

void BoxCollider2D::update(float dt) 
{
    Collider2D::update(dt);   
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

    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_size(rigidbody.body(), {width_, height_, 0.0f});

    return *this;
}

float BoxCollider2D::height() const noexcept 
{
    return height_;
}

BoxCollider2D& BoxCollider2D::height(float value) noexcept 
{
    height_ = value;

    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_size(rigidbody.body(), {width_, height_, 0.0f});

    return *this;
}

BoxCollider2D& BoxCollider2D::friction(float value) noexcept 
{
    Collider2D::friction(value);
    
    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_friction(rigidbody.body(), value, ShapeType2D::Polygon);

    return *this;
}

BoxCollider2D& BoxCollider2D::bounciness(float value) noexcept 
{
    Collider2D::bounciness(value);
    
    auto& rigidbody = get_rigidbody().get();
    Body2D::set_body_bounciness(rigidbody.body(), value, ShapeType2D::Polygon);

    return *this;
}