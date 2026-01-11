#pragma once

#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/public/component.h>

constexpr float default_rigidbody_mass = 1.0f;
constexpr bool default_rigidbody_use_gravity = true;
constexpr float default_rigidbody_gravity_scale = 1.0f;

/**
 * @brief A 2D rigidbody component that adds physics properties to a GameObject.
 *
 * This component allows a GameObject to interact with the physics engine,
 * enabling features such as gravity, collisions, and movement based on physical
 * forces.
 *
 * Usage:
 * - Create a Rigidbody2D component and attach it to a GameObject.
 * - Configure properties such as mass, gravity usage, and body type.
 * - Use methods like apply_force() and apply_impulse() to influence movement.
 * @note Ensure the GameObject has a Collider2D component for proper physics
 */
class Rigidbody2D : public Component {
 public:
  Rigidbody2D(BodyType2D::Type type = BodyType2D::Dynamic,
              float mass = default_rigidbody_mass,
              bool use_gravity = default_rigidbody_use_gravity,
              float gravity_scale = default_rigidbody_gravity_scale);
  ~Rigidbody2D() override;

  void update(float dt) override;

  Rigidbody2D& teleport(const Vector3& position) noexcept;

  [[nodiscard]] BodyType2D::Type type() const noexcept;
  Rigidbody2D& type(BodyType2D::Type value) noexcept;

  [[nodiscard]] float mass() const noexcept;
  Rigidbody2D& mass(float value) noexcept;

  [[nodiscard]] bool use_gravity() const noexcept;
  Rigidbody2D& use_gravity(bool value) noexcept;

  [[nodiscard]] float gravity_scale() const noexcept;
  Rigidbody2D& gravity_scale(float value) noexcept;

  [[nodiscard]] Body2D body() const noexcept;
  void body(const Body2D& value) noexcept;

  /** @brief Applies a force to the rigidbody.
   *
   * @param force The force vector to apply.
   */
  void apply_force(const Vector3& force) noexcept;

  /** @brief Applies an impulse to the rigidbody.
   *
   * @param impulse The impulse vector to apply.
   */
  void apply_impulse(const Vector3& impulse) noexcept;

  /** @brief Retrieves the current velocity of the rigidbody.
   *
   * @return Vector3 The current velocity vector.
   */
  void velocity(const Vector3& value) noexcept;

  /** @brief Retrieves the current velocity of the rigidbody.
   *
   * @return Vector3 The current velocity vector.
   */
  [[nodiscard]] Vector3 velocity() const noexcept;

  void mark_network_dirty() noexcept;

  void on_serialize(std::vector<uint8_t>& out) const override;
  void on_deserialize(const std::vector<uint8_t>& data,
                      size_t& offset) override;

  std::string type_name() const override;

 private:
  Body2D body_{};
  BodyType2D::Type type_;
  float mass_;

  bool use_gravity_;
  float gravity_scale_;
  bool fresh_spawned_{true};
};