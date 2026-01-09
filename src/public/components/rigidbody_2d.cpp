#include <engine/core/engine.h>
#include <engine/network/snapshot.h>
#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/physics/physics_service.h>
#include <engine/public/components/network_identity.h>
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

      if (use_gravity_) Body2D::set_body_gravity_scale(body_, gravity_scale_);
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

    mark_network_dirty();
  }

  return *this;
}

BodyType2D::Type Rigidbody2D::type() const noexcept { return type_; }

Rigidbody2D& Rigidbody2D::type(BodyType2D::Type value) noexcept {
  type_ = value;
  Body2D::set_body_type(body_, type_);

  mark_network_dirty();

  return *this;
}

float Rigidbody2D::mass() const noexcept { return mass_; }

Rigidbody2D& Rigidbody2D::mass(float value) noexcept {
  mass_ = value;
  Body2D::set_body_mass(body_, mass_);

  mark_network_dirty();

  return *this;
}

bool Rigidbody2D::use_gravity() const noexcept { return use_gravity_; }

Rigidbody2D& Rigidbody2D::use_gravity(bool value) noexcept {
  use_gravity_ = value;
  Body2D::set_body_gravity_scale(body_, use_gravity_ ? gravity_scale_ : 0.0f);

  mark_network_dirty();

  return *this;
}

float Rigidbody2D::gravity_scale() const noexcept { return gravity_scale_; }

Rigidbody2D& Rigidbody2D::gravity_scale(float value) noexcept {
  gravity_scale_ = value;
  Body2D::set_body_gravity_scale(body_, gravity_scale_);

  mark_network_dirty();

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

  mark_network_dirty();
}

void Rigidbody2D::apply_impulse(const Vector3& impulse) noexcept {
  Body2D::apply_impulse(body_, impulse);

  mark_network_dirty();
}

void Rigidbody2D::velocity(const Vector3& value) noexcept {
  Body2D::set_body_velocity(body_, value);

  mark_network_dirty();
}

Vector3 Rigidbody2D::velocity() const noexcept {
  Vector3 v = Body2D::get_body_velocity(body_);
  return {v.x, v.y, 0.0f};
}

void Rigidbody2D::mark_network_dirty() noexcept {
  if (auto parent_opt = parent(); parent_opt.has_value()) {
    if (auto network_id = parent_opt->get().get_component<NetworkIdentity>()) {
      network_id.value().get().mark_dirty();
    }
  }
}

void Rigidbody2D::on_serialize(std::vector<uint8_t>& out) const {
  Vector3 pos = Body2D::get_pixel_transform(body_).position;
  snapshot::write_bytes(out, &pos, sizeof(Vector3));

  Vector3 vel = velocity();
  snapshot::write_bytes(out, &vel, sizeof(Vector3));

  float rot = Body2D::get_pixel_transform(body_).rotation;
  snapshot::write_bytes(out, &rot, sizeof(float));

  uint32_t type_val = static_cast<uint32_t>(type_);
  snapshot::write_bytes(out, &type_val, sizeof(uint32_t));

  snapshot::write_bytes(out, &mass_, sizeof(float));

  uint8_t use_gravity_val = use_gravity_ ? 1 : 0;
  snapshot::write_bytes(out, &use_gravity_val, sizeof(uint8_t));

  snapshot::write_bytes(out, &gravity_scale_, sizeof(float));
}

void Rigidbody2D::on_deserialize(const std::vector<uint8_t>& data,
                                 size_t& offset) {
  Vector3 net_pos;
  if (!snapshot::read_bytes(data, offset, &net_pos, sizeof(Vector3))) return;

  Vector3 net_vel;
  if (!snapshot::read_bytes(data, offset, &net_vel, sizeof(Vector3))) return;

  float rot;
  if (!snapshot::read_bytes(data, offset, &rot, sizeof(float))) return;

  uint32_t type_val;
  if (!snapshot::read_bytes(data, offset, &type_val, sizeof(uint32_t))) return;
  BodyType2D::Type type = static_cast<BodyType2D::Type>(type_val);

  float mass;
  if (!snapshot::read_bytes(data, offset, &mass, sizeof(float))) return;

  uint8_t use_gravity_val;
  if (!snapshot::read_bytes(data, offset, &use_gravity_val, sizeof(uint8_t)))
    return;
  bool use_gravity = use_gravity_val != 0;

  float gravity_scale;
  if (!snapshot::read_bytes(data, offset, &gravity_scale, sizeof(float)))
    return;

  Vector3 cur_pos = Body2D::get_pixel_transform(body_).position;
  Vector3 cur_vel = velocity();

  Vector3 delta = net_pos - cur_pos;

  /// If freshly spawned, teleport directly to position
  /// that way we avoid massive rubberbanding as an object
  /// would be catapulted from (0,0,0) to the correct position
  if (fresh_spawned_) {
    teleport(net_pos);
    velocity(net_vel);
    fresh_spawned_ = false;
  }
  /// Otherwise, smoothly correct position and velocity
  /// using a simple lerp: linear interpolation
  ///
  /// The lerp works by moving a fraction of the distance
  /// between the current position and the target position
  /// each update, resulting in a smooth transition.
  ///
  /// This helps to reduce sudden jumps or rubberbanding
  /// effects in networked physics simulations.
  else {
    const float POSITION_CORRECTION_THRESHOLD = 0.2f;
    const float lerpFactor = 0.4f;

    if (fabs(delta.x) > POSITION_CORRECTION_THRESHOLD ||
        fabs(delta.y) > POSITION_CORRECTION_THRESHOLD) {
      Vector3 corrected = cur_pos + delta * lerpFactor;
      teleport(corrected);
    }

    /// Smoothly interpolate velocity to avoid sudden changes.
    /// The calculation is similar to position correction =>
    /// current velocity + (target velocity - current velocity) * lerpFactor
    Vector3 smoothedVel;
    smoothedVel.x = cur_vel.x + (net_vel.x - cur_vel.x) * lerpFactor;
    smoothedVel.y = cur_vel.y + (net_vel.y - cur_vel.y) * lerpFactor;

    velocity(smoothedVel);
  }

  mass_ = mass;
  use_gravity_ = use_gravity;
  gravity_scale_ = gravity_scale;

  mark_network_dirty();
}

std::string Rigidbody2D::type_name() const { return "Rigidbody2D"; }