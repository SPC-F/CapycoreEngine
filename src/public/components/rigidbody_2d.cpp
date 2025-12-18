#include <engine/core/engine.h>
#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/physics/physics_service.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/gameObject.h>

#include <stdexcept>

Rigidbody2D::Rigidbody2D(BodyType2D::Type type, float mass, bool use_gravity,
                         float gravity_scale)
    : type_(type),
      mass_(mass),
      use_gravity_(use_gravity),
      gravity_scale_(gravity_scale) {
  auto on_awake = [this](Component& comp) {
    auto& physics_service =
        Engine::instance().services->get_service<PhysicsService>().get();

    if (auto parent_opt = parent(); !parent_opt.has_value()) {
      throw std::runtime_error("Rigidbody2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
      auto& transform = parent()->get().transform();
      body_ = physics_service.factory()->create_body(transform.position(),
                                                     type_, this);

      if (body_.id.index1 == -1) {
        throw std::runtime_error("Failed to create Rigidbody2D body.");
      }
    }
  };

  add_on_attach(on_awake);

  auto on_destroy = [this](Component& comp) {
    PhysicsCreationFactory::destroy_body(body_);
  };

  add_on_detach(on_destroy);
}

Rigidbody2D::~Rigidbody2D() = default;

void Rigidbody2D::update(float dt) {
  auto& gameobject = parent()->get();
  auto& tf = gameobject.transform();

  /// Dynamic bodies: Update GameObject transform from physics body
  if (type_ == BodyType2D::Dynamic) {
    Body2DTransform body_tf = Body2D::get_pixel_transform(body_);
    body_tf.position = PhysicsMath::physics_vec3_to_transform_pixel_vec3(
        body_tf.position, 0, 0);

    tf.position(body_tf.position);
    tf.rotation(body_tf.rotation);
  }
  /// Kinematic bodies: Update physics body from GameObject transform
  else if (type_ == BodyType2D::Kinematic) {
    Vector3 world_pos = tf.position();
    float world_rot = tf.rotation();

    Body2DTransform body_tf = Body2D::get_pixel_transform(body_);
    body_tf.position = world_pos;
    body_tf.rotation = world_rot;

    Body2D::set_body_transform(body_tf, true);
  }
  /// Static bodies: Do nothing
}

Rigidbody2D& Rigidbody2D::teleport(const Vector3& position) noexcept {
  if (const auto parent_opt = parent(); parent_opt.has_value()) {
    Body2DTransform b_transform = Body2D::get_pixel_transform(body_);
    b_transform.position = position;
    Body2D::set_body_transform(b_transform, true);

    parent_opt->get().transform().position(b_transform.position);
  }

  return *this;
}

BodyType2D::Type Rigidbody2D::type() const noexcept { return type_; }

Rigidbody2D& Rigidbody2D::type(BodyType2D::Type value) noexcept {
  type_ = value;
  Body2D::set_body_type(body_, type_);
  return *this;
}

float Rigidbody2D::mass() const noexcept { return mass_; }

Rigidbody2D& Rigidbody2D::mass(float value) noexcept {
  mass_ = value;
  Body2D::set_body_mass(body_, mass_);
  return *this;
}

bool Rigidbody2D::use_gravity() const noexcept { return use_gravity_; }

Rigidbody2D& Rigidbody2D::use_gravity(bool value) noexcept {
  use_gravity_ = value;
  Body2D::set_body_gravity_scale(body_, use_gravity_ ? gravity_scale_ : 0.0f);
  return *this;
}

float Rigidbody2D::gravity_scale() const noexcept { return gravity_scale_; }

Rigidbody2D& Rigidbody2D::gravity_scale(float value) noexcept {
  gravity_scale_ = value;
  Body2D::set_body_gravity_scale(body_, gravity_scale_);
  return *this;
}

Body2D Rigidbody2D::body() const noexcept { return body_; }

void Rigidbody2D::body(const Body2D& value) noexcept {
  if (body_.id.index1 != value.id.index1) {
    PhysicsCreationFactory::destroy_body(body_);
  }

  body_ = value;
}

void Rigidbody2D::apply_force(const Vector3& force) noexcept {
  Body2D::apply_force(body_, force);
}

void Rigidbody2D::apply_impulse(const Vector3& impulse) noexcept {
  Body2D::apply_impulse(body_, impulse);
}

void Rigidbody2D::velocity(const Vector3& value) noexcept {
  Body2D::set_body_velocity(body_, value);
}

Vector3 Rigidbody2D::velocity() const noexcept {
  Vector3 v = Body2D::get_body_velocity(body_);
  return {v.x, v.y, 0.0f};
}

std::string Rigidbody2D::type_name() const { return "Rigidbody2D"; }