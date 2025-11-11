#pragma once

#include <box2d/box2d.h>

/**
 * @brief Namespace containing mathematical utility functions for physics calculations.
 * 
 * This namespace provides functions to convert between different units
 * (pixels to meters, degrees to radians, etc.) commonly used in physics simulations.
 */
namespace PhysicsMath {
    /** @brief Convert pixels to meters using the specified ratio */
    float to_meters(float pixels, float ratio) noexcept;
    /** @brief Convert meters to pixels using the specified ratio */
    float to_pixels(float meters, float ratio) noexcept;
    /** @brief Convert degrees to radians */
    float to_radians(float degrees) noexcept;
    /** @brief Convert radians to degrees */
    float to_degrees(float radians) noexcept;
};