#pragma once
#include <engine/public/util/vector3.h>

#include <memory>
#include <optional>

/**
 * @brief Represents the position, rotation, and scale of a GameObject in 3D
 * space. The Transform class encapsulates the local and world transformations
 * of a GameObject, including its position, rotation, and scale. It also
 * supports hierarchical relationships through parent-child transforms.
 *
 * @note The position is represented as a Vector3, the rotation as a float (in
 * degrees), and the scale as a Vector3.
 * @note The parent transform is optional, allowing for both root and child
 * transforms.
 * @note Modifying the local position, rotation, or scale will affect the world
 * transformation based on the parent's transformation.
 * @note This class does not handle matrix transformations; it focuses on the
 * basic representation of position, rotation, and scale.
 */
class Transform {
 private:
  Vector3 local_position_;
  float rotation_{};
  Vector3 scale_;
  std::optional<std::reference_wrapper<Transform>> parent_;

 public:
  Transform();
  explicit Transform(Vector3 position);
  Transform(Vector3 position, float rotation, Vector3 scale);
  Transform(Vector3 position, float rotation, Vector3 scale,
            std::optional<std::reference_wrapper<Transform>> parent);

  /**
   * @brief Get the local position of the Transform.
   * @return The local position as a Vector3.
   */
  [[nodiscard]] Vector3 local_position() const noexcept;

  /**
   * @brief Set the local position of the Transform.
   * @param pos The new local position as a Vector3.
   * @return A reference to this Transform for chaining.
   */
  Transform& local_position(const Vector3& pos) noexcept;

  /**
   * @brief Get the world position of the Transform.
   * @return The world position as a Vector3.
   */
  [[nodiscard]] Vector3 position() const noexcept;

  /**
   * @brief Set the world position of the Transform.
   * @param pos The new world position as a Vector3.
   * @return A reference to this Transform for chaining.
   */
  Transform& position(const Vector3& pos) noexcept;

  /**
   * @brief Set the rotation of the Transform in degrees.
   * @param rot The new rotation in degrees.
   * @return A reference to this Transform for chaining.
   */
  Transform& rotation(float rot) noexcept;

  /**
   * @brief Get the rotation of the Transform in degrees.
   * @return The rotation in degrees.
   */
  [[nodiscard]] float rotation() const noexcept;

  /**
   * @brief Set the scale of the Transform.
   * @param scale The new scale as a Vector3.
   * @return A reference to this Transform for chaining.
   */
  Transform& scale(Vector3 scale) noexcept;

  /**
   * @brief Get the scale of the Transform.
   * @return The scale as a Vector3.
   */
  [[nodiscard]] Vector3 scale() const noexcept;

  /**
   * @brief Set the parent Transform.
   * @param parent An optional reference to the new parent Transform.
   * @return A reference to this Transform for chaining.
   */
  Transform& parent(
      std::optional<std::reference_wrapper<Transform>> parent) noexcept;

  /**
   * @brief Get the parent Transform.
   * @return An optional reference to the parent Transform.
   */
  [[nodiscard]] std::optional<std::reference_wrapper<Transform>> parent()
      const noexcept;
};
