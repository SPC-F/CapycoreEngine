#pragma once

#include <box2d/box2d.h>
#include <cstdint>

/**
 * @brief Structure representing a 2D physics body.
 * 
 * This struct encapsulates the Box2D body identifier
 * along with its associated shapes.
 */
struct ShapeType2D
{
    enum Type : int8_t
    {
        Circle = 0,
        Polygon = 1,
    };
    
    /** @brief Converts the custom ShapeType2D::Type to the corresponding Box2D shape type. */
    static b2ShapeType to_box2d_type(Type type)
    {
        switch (type)
        {
            case Type::Circle:
                return b2ShapeType::b2_circleShape;
            case Type::Polygon:
            default:
                return b2ShapeType::b2_polygonShape;
        }
    }
};