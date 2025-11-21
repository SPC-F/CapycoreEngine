#pragma once

#include <box2d/box2d.h>
#include <engine/public/util/point.h>

/**
 * @brief Namespace containing mathematical utility functions for physics calculations.
 * 
 * This namespace provides functions to convert between different units
 * (pixels to meters, degrees to radians, etc.) commonly used in physics simulations.
 */
namespace PhysicsMath 
{
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
};