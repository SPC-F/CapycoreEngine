#pragma once

/**
 * @brief Represents a 3D vector with x, y, and z components.
 * This class provides basic vector operations such as addition,
 * subtraction, multiplication, division, normalization, and length calculation.
 */
class Vector3 {
 public:
  float x;
  float y;
  float z;

  Vector3();
  Vector3(float x, float y, float z);

  Vector3 operator+(const Vector3& other) const noexcept;
  Vector3 operator-(const Vector3& other) const noexcept;
  Vector3 operator*(const Vector3& other) const noexcept;
  Vector3 operator/(const Vector3& other) const noexcept;

  Vector3& operator+=(const Vector3& other) noexcept;
  Vector3& operator-=(const Vector3& other) noexcept;
  Vector3& operator*=(const Vector3& other) noexcept;
  Vector3& operator/=(const Vector3& other) noexcept;

  Vector3 operator*(float value) const noexcept;
  Vector3 operator/(float value) const noexcept;

  Vector3& operator*=(float value) noexcept;
  Vector3& operator/=(float value) noexcept;

  /**
   * @brief Calculates the length (magnitude) of the vector.
   * @return The length of the vector as a float.
   */
  float length() const noexcept;

  /**
   * @brief Normalizes the vector to have a length of 1.
   * @return void
   */
  void normalize() noexcept;

  /**
   * @brief Checks if this vector is equal to another vector.
   * @param other The other vector to compare with.
   * @return true if the vectors are equal, false otherwise.
   */
  bool equals(const Vector3& other) const noexcept;
};
