#pragma once

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

  /// Calculate the length (magnitude) of the vector
  float length() const noexcept;

  /// Normalize the vector to a unit vector
  void normalize() noexcept;
};
