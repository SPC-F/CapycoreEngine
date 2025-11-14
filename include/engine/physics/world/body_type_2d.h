#pragma once

#include <cstdint>

/**
 * @brief Enumeration of 2D body types.
 * 
 * This struct defines the different types of physics bodies
 * that can be created in a 2D physics world.
 */
struct BodyType2D
{
    enum Type : uint8_t
    {
        Static = 0,
        Dynamic = 1,
        Kinematic = 2
    };
};