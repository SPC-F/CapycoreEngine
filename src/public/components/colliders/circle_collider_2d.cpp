#include <engine/public/components/colliders/circle_collider_2d.h>
#include <engine/public/gameObject.h>

#include <stdexcept>

CircleCollider2D::CircleCollider2D(float friction, float bounciness,
                                   float radius, Point offset, bool is_sensor,
                                   bool is_bullet)
    : Collider2D(friction, bounciness, offset), radius_(radius) {
  auto on_awake = [this, offset, friction, bounciness, is_sensor,
                   is_bullet](Component& comp) {
    if (!parent().has_value()) {
      throw std::runtime_error("CircleCollider2D has no parent GameObject.");
    }

    if (auto gameobject_opt = parent(); gameobject_opt.has_value()) {
      auto& gameobject = gameobject_opt->get();

      auto rigidbody_opt = gameobject.get_component<Rigidbody2D>();
      auto circle_collider_opt = gameobject.get_component<CircleCollider2D>();

      if (!rigidbody_opt.has_value()) {
        throw std::runtime_error(
            "CircleCollider2D requires a Rigidbody2D component on the same "
            "GameObject.");
      }

      if (circle_collider_opt.has_value()) {
        throw std::runtime_error(
            "GameObject cannot have multiple CircleCollider2D components.");
      }

      auto& rigidbody = rigidbody_opt->get();
      auto body = rigidbody.body();

      auto flags = Collider2D::creation_flags();
      flags.desired_mass = rigidbody.mass();
      flags.bounciness = bounciness;
      flags.friction = friction;
      flags.sensor = is_sensor;
      flags.is_bullet = is_bullet;
      creation_flags_ = flags;

      rigidbody.body(PhysicsCreationFactory::create_circle_fixture(
          body, offset, radius_, flags));
    }
  };

  add_on_attach(on_awake);
}

void CircleCollider2D::update(float dt) { Collider2D::update(dt); }

float CircleCollider2D::radius() const noexcept { return radius_; }

CircleCollider2D& CircleCollider2D::radius(float value) noexcept {
  radius_ = value;

  auto& rigidbody = get_rigidbody().get();
  Body2D::set_body_radius(rigidbody.body(), radius_, offset());

  return *this;
}

CircleCollider2D& CircleCollider2D::friction(float value) noexcept {
  Collider2D::friction(value);

  auto& rigidbody = get_rigidbody().get();
  Body2D::set_body_friction(rigidbody.body(), value, ShapeType2D::Circle);

  return *this;
}

CircleCollider2D& CircleCollider2D::bounciness(float value) noexcept {
  Collider2D::bounciness(value);

  auto& rigidbody = get_rigidbody().get();
  Body2D::set_body_bounciness(rigidbody.body(), value, ShapeType2D::Circle);

  return *this;
}

std::string CircleCollider2D::type_name() const { return "CircleCollider2D"; }

Point CircleCollider2D::offset() const noexcept { return Collider2D::offset(); }

CircleCollider2D& CircleCollider2D::offset(Point value) noexcept {
  Collider2D::offset(value);

  auto& rigidbody = get_rigidbody().get();
  Body2D::set_body_radius(rigidbody.body(), radius_, offset());

  return *this;
}
