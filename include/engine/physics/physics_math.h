#pragma once

#include <box2d/box2d.h>
#include <engine/physics/world/physics_world.h>
#include <engine/public/util/point.h>
#include <engine/public/util/vector3.h>

/**
 * @brief Namespace containing mathematical utility functions for physics
 * calculations.
 *
 * This namespace provides functions to convert between different units
 * (pixels to meters, degrees to radians, etc.) commonly used in physics
 * simulations.
 */
namespace PhysicsMath {
static constexpr float pi = 3.14159265358979323846f;
static constexpr float circle_divisor = 180.0f;

/** @brief Convert pixels to meters using the specified ratio */
float to_meters(float pixels, float ratio) noexcept;

/** @brief Convert meters to pixels using the specified ratio */
float to_pixels(float meters, float ratio) noexcept;

/** @brief Convert degrees to radians */
float to_radians(float degrees) noexcept;

/** @brief Convert radians to degrees */
float to_degrees(float radians) noexcept;

/** @brief Convert Box2D angle representation (sine, cosine) to radians */
float convert_box2d_angle_to_radians(float sine, float cosine) noexcept;

/** @brief Convert radians to Box2D sine value */
float convert_radians_to_box2d_sine(float radians) noexcept;

/** @brief Convert radians to Box2D cosine value */
float convert_radians_to_box2d_cosine(float radians) noexcept;

/** @brief Calculate the distance between two 2D points */
float distance_between_points(const Point& a, const Point& b) noexcept;

/** @brief Calculate the difference in x-coordinates between two 2D points */
float dx_between_points(const Point& a, const Point& b) noexcept;

/** @brief Calculate the difference in y-coordinates between two 2D points */
float dy_between_points(const Point& a, const Point& b) noexcept;

/** @brief Convert a Vector3 to Box2D vector representation */
Vector3 box2d_vec_to_pixel_vec3(const Vector3& v, bool to_pixels = true);

/** @brief Convert a Box2D vector representation to Vector3 */
Vector3 vec3_to_box2d_vec3(const Vector3& v, bool from_pixels = true);

/** @brief Convert length from Box2D units to pixels */
float box2d_to_pixels(float length) noexcept;

/** @brief Convert length from pixels to Box2D units */
float pixels_to_box2d(float length) noexcept;

/** @brief Convert physics Vector3 to transform pixel Vector3 */
Vector3 physics_vec3_to_transform_pixel_vec3(const Vector3& v, float width,
                                             float height) noexcept;
};  // namespace PhysicsMath