#pragma once

#include <box2d/box2d.h>
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

    /** @brief Converts the custom BodyType2D::Type to the corresponding Box2D body type. */
    static b2BodyType to_box2d_body_type(BodyType2D::Type type) 
    {
        switch (type) {
            case Dynamic:
                return b2_dynamicBody;
            case Kinematic:
                return b2_kinematicBody;
            case Static:
            default:
                return b2_staticBody;
        }
    }
};