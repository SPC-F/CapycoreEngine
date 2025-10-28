#include <engine/public/util/vector3.h>

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3& right) const noexcept {
    return {x + right.x, y + right.y, z + right.z};
}
Vector3 Vector3::operator-(const Vector3& right) const noexcept {
    return {x - right.x, y - right.y, z - right.z};
}

Vector3& Vector3::operator+=(const Vector3&& right) noexcept {
    x += right.x;
    y += right.y;
    z += right.z;

    return *this;
}
Vector3 Vector3::operator-=(const Vector3&& right) noexcept {
    x = x - right.x;
    y = y - right.y;
    z = z - right.z;

    return *this;
}