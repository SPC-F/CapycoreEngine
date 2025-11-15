#include <engine/public/components/colliders/collider_2d.h>

#include <stdexcept>

#include <engine/core/engine.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/raycast/physics_raycaster.h>
#include <engine/physics/physics_service.h>
#include <engine/physics/world/physics_world.h>
#include <engine/public/gameObject.h>

Collider2D::Collider2D(
    float friction, 
    float bounciness
) : friction_(friction), 
    bounciness_(bounciness) 
{
    if (!parent().has_value()) {
        throw std::runtime_error("Collider2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
        auto& gameobject = gameobject_opt->get();

        if (!gameobject.get_component<Rigidbody2D>().has_value()) {
            throw std::runtime_error("Collider2D requires a Rigidbody2D component on the same GameObject.");
        }
    }
}

void Collider2D::update(float dt) 
{
    auto gameobject_opt = parent();
    if (!gameobject_opt.has_value()) {
        throw std::runtime_error("Collider2D has no parent GameObject.");
    }
    
    auto& gameobject = gameobject_opt->get();
    if (!gameobject.get_component<Rigidbody2D>().has_value()) {
        throw std::runtime_error("Collider2D requires a Rigidbody2D component on the same GameObject.");
    }

    auto& rigidbody = get_rigidbody().get();

    auto& physics_service = Engine::instance().services->get_service<PhysicsService>().get();
    auto& physics_world = physics_service.world();

    Body2DTransform transform = Body2D::get_body_transform(rigidbody.body());
    gameobject.transform().position(transform.position);
    gameobject.transform().rotation(transform.rotation);
}

void Collider2D::on_trigger_enter(Collider2D& other) 
{
    for (auto& action : on_trigger_enter_actions_) {
        action(*this, other);
    }
}

void Collider2D::on_trigger_exit(Collider2D& other) 
{
    for (auto& action : on_trigger_exit_actions_) {
        action(*this, other);
    }
}

void Collider2D::on_collision_enter(Collider2D& other) 
{
    for (auto& action : on_collision_enter_actions_) {
        action(*this, other);
    }
}

void Collider2D::on_collision_exit(Collider2D& other) 
{
    for (auto& action : on_collision_exit_actions_) {
        action(*this, other);
    }
}

BodyDistance2D Collider2D::distance(Collider2D& other, bool use_fixture) 
{
    auto& physics_service = Engine::instance().services->get_service<PhysicsService>().get();
    auto& physics_world = physics_service.world();   

    auto rigidbody_a = get_rigidbody().get();
    auto rigidbody_b = other.get_rigidbody().get();

    Body2DTransform transform_a = Body2D::get_body_transform(rigidbody_a.body());
    Body2DTransform transform_b = Body2D::get_body_transform(rigidbody_b.body());

    if (use_fixture) {
        return PhysicsWorld::fixture_distance(transform_a, transform_b);
    }
    
    return PhysicsWorld::distance(transform_a, transform_b);
}

ColliderRayResult Collider2D::raycast(const Point& direction, float max_distance) 
{
    auto& physics_service = Engine::instance().services->get_service<PhysicsService>().get();
    auto& physics_raycaster = physics_service.raycaster();

    auto rigidbody = get_rigidbody().get();
    Body2DTransform body_transform = Body2D::get_body_transform(rigidbody.body());
    b2Vec2 origin {body_transform.position.x, body_transform.position.y};
    b2Vec2 translation {direction.x * max_distance, direction.y * max_distance};

    return physics_raycaster->raycast_closest(origin, translation);
}

std::reference_wrapper<Rigidbody2D> Collider2D::get_rigidbody() 
{
    auto parent_opt = parent();
    if (!parent_opt.has_value()) {
        throw std::runtime_error("Collider2D has no parent GameObject.");
    }

    auto rigidbody_opt = parent_opt->get().get_component<Rigidbody2D>();
    if (!rigidbody_opt.has_value()) {
        throw std::runtime_error("No Rigidbody2D component found on the parent GameObject.");
    }

    return rigidbody_opt->get();
}

float Collider2D::friction() const noexcept {
    return friction_;
}

Collider2D& Collider2D::friction(float value) noexcept {
    friction_ = value;
    return *this;
}

float Collider2D::bounciness() const noexcept {
    return bounciness_;
}

Collider2D& Collider2D::bounciness(float value) noexcept {
    bounciness_ = value;
    return *this;
}

PhysicsCreationFlags& Collider2D::creation_flags() noexcept {
    return creation_flags_;
}

Collider2D& Collider2D::creation_flags(PhysicsCreationFlags value) noexcept {
    creation_flags_ = value;
    return *this;
}