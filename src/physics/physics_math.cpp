#include <engine/physics/physics_math.h>
#include <engine/physics/world/body/shape_type_2d.h>

namespace PhysicsMath {
float to_meters(float pixels, float ratio) noexcept { return pixels / ratio; }

float to_pixels(float meters, float ratio) noexcept { return meters * ratio; }

float to_radians(float degrees) noexcept {
  return degrees * (B2_PI / 180.0f);  // NOLINT
}

float to_degrees(float radians) noexcept {
  return radians * (180.0f / B2_PI);  // NOLINT
}

float convert_box2d_angle_to_radians(float sine, float cosine) noexcept {
  return std::atan2(sine, cosine);
}

float convert_radians_to_box2d_sine(float radians) noexcept {
  return std::sin(radians);
}

float convert_radians_to_box2d_cosine(float radians) noexcept {
  return std::cos(radians);
}

float distance_between_points(const Point& a, const Point& b) noexcept {
  float dx = dx_between_points(a, b);
  float dy = dy_between_points(a, b);
  return std::sqrt((dx * dx) + (dy * dy));
}

float dx_between_points(const Point& a, const Point& b) noexcept {
  return b.x - a.x;
}

float dy_between_points(const Point& a, const Point& b) noexcept {
  return b.y - a.y;
}

Vector3 box2d_vec_to_pixel_vec3(const Vector3& v, bool to_pixels) {
  float scale = to_pixels ? PhysicsWorld::pixels_per_meters : 1.0f;
  return Vector3{v.x * scale, v.y * scale, 0.0f};
}

Vector3 vec3_to_box2d_vec3(const Vector3& v, bool from_pixels) {
  float scale = from_pixels ? PhysicsWorld::meters_per_pixels : 1.0f;
  return Vector3{v.x * scale, v.y * scale, 0.0f};
}

float box2d_to_pixels(float length) noexcept {
  return length * PhysicsWorld::pixels_per_meters;
}

float pixels_to_box2d(float length) noexcept {
  return length * PhysicsWorld::meters_per_pixels;
}

Vector3 physics_vec3_to_transform_pixel_vec3(const Vector3& v, float width_px,
                                             float height_px) noexcept {
  float x = v.x + (width_px / 2.0f);
  float y = v.y + (height_px / 2.0f);
  return Vector3{x, y, 0.0f};
}
}  // namespace PhysicsMath