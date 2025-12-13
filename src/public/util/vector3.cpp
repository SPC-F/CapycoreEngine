#include <engine/public/util/vector3.h>

#include <cmath>

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3& other) const noexcept {
  return Vector3{x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3& other) const noexcept {
  return Vector3{x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator*(const Vector3& other) const noexcept {
  return Vector3{x * other.x, y * other.y, z * other.z};
}

Vector3 Vector3::operator/(const Vector3& other) const noexcept {
  return Vector3{x / other.x, y / other.y, z / other.z};
}

Vector3& Vector3::operator+=(const Vector3& other) noexcept {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) noexcept {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

Vector3& Vector3::operator*=(const Vector3& other) noexcept {
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return *this;
}

Vector3& Vector3::operator/=(const Vector3& other) noexcept {
  x /= other.x;
  y /= other.y;
  z /= other.z;
  return *this;
}

Vector3 Vector3::operator*(float v) const noexcept {
  return Vector3{x * v, y * v, z * v};
}

Vector3 Vector3::operator/(float v) const noexcept {
  return Vector3{x / v, y / v, z / v};
}

Vector3& Vector3::operator*=(float v) noexcept {
  x *= v;
  y *= v;
  z *= v;
  return *this;
}

Vector3& Vector3::operator/=(float v) noexcept {
  x /= v;
  y /= v;
  z /= v;
  return *this;
}

float Vector3::length() const noexcept {
  return std::sqrt(x * x + y * y + z * z);
}

void Vector3::normalize() noexcept {
  float len = length();
  if (len > 0.0f) {
    x /= len;
    y /= len;
    z /= len;
  }
}

bool Vector3::equals(const Vector3& other) const noexcept {
  return x == other.x && y == other.y && z == other.z;
}