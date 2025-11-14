#include <engine/public/components/rigidbody_2d.h>

#include <stdexcept>

#include <engine/core/engine.h>
#include <engine/public/gameObject.h>
#include <engine/physics/physics_service.h>
#include <engine/physics/creation/physics_creation_factory.h>

Rigidbody2D::Rigidbody2D(
    BodyType2D::Type type,
    float mass,
    bool use_gravity,
    float gravity_scale_x,
    float gravity_scale_y
) : type_(type),
    mass_(mass),
    use_gravity_(use_gravity),
    gravity_scale_x_(gravity_scale_x),
    gravity_scale_y_(gravity_scale_y) 
{
    add_on_attach([this](Component& comp) {
        auto& physics_service = Engine::instance().services->get_service<PhysicsService>().get();

        if (auto parent_opt = parent(); !parent_opt.has_value()) {
            throw std::runtime_error("Rigidbody2D has no parent GameObject.");
        }
     
        if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
            auto& transform = parent()->get().transform();
            body_ = physics_service.factory()->create_body(transform.position(), type_, this);

            if (body_.id.index1 == -1) {
                throw std::runtime_error("Failed to create Rigidbody2D body.");
            }
        }
    });

    add_on_detach([this](Component& comp) {
        PhysicsCreationFactory::destroy_body(body_);
    });
}

Rigidbody2D::~Rigidbody2D() = default;

void Rigidbody2D::update() 
{
    auto& physics_service = Engine::instance().services->get_service<PhysicsService>().get();
    
    if (auto parent_opt = parent(); parent_opt.has_value()) {
        auto& gameobject = parent_opt->get();
        Body2DTransform transform = PhysicsWorld::get_body_transform(body_);

        gameobject.transform().position(transform.position);
        gameobject.transform().rotation(transform.rotation);
    }
}

void Rigidbody2D::on_serialize() 
{
    // TODO: Implement after network...
}

void Rigidbody2D::on_deserialize() 
{
    // TODO: Implement after network...   
}

BodyType2D::Type Rigidbody2D::type() const noexcept 
{
    return type_;
}

Rigidbody2D& Rigidbody2D::type(BodyType2D::Type value) noexcept 
{
    type_ = value;
    return *this;
}

float Rigidbody2D::mass() const noexcept 
{
    return mass_;
}

Rigidbody2D& Rigidbody2D::mass(float value) noexcept 
{
    mass_ = value;
    return *this;
}

bool Rigidbody2D::use_gravity() const noexcept 
{
    return use_gravity_;
}

Rigidbody2D& Rigidbody2D::use_gravity(bool value) noexcept 
{
    use_gravity_ = value;
    return *this;
}

float Rigidbody2D::gravity_scale_x() const noexcept 
{
    return gravity_scale_x_;
}

Rigidbody2D& Rigidbody2D::gravity_scale_x(float value) noexcept 
{
    gravity_scale_x_ = value;
    return *this;
}

float Rigidbody2D::gravity_scale_y() const noexcept 
{
    return gravity_scale_y_;
}

Rigidbody2D& Rigidbody2D::gravity_scale_y(float value) noexcept 
{
    gravity_scale_y_ = value;
    return *this;
}

Body2D Rigidbody2D::body() const noexcept 
{
    return body_;
}