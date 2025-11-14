#pragma once

#include <box2d/box2d.h>
#include <vector>

#include <engine/physics/world/body/shape_type_2d.h>
#include <engine/physics/world/body/body_type_2d.h>
#include <engine/public/util/vector3.h>

/**
 * @brief Structure representing a 2D physics shape.
 * 
 * Includes the shape ID and type.
 */
struct Shape2D {
    b2ShapeId id {};
    b2ShapeType type {};
};

/**
 * @brief Structure representing a 2D physics body.
 * 
 * Includes the body ID and associated shapes.
 */
struct Body2D {
    b2BodyId id {};
    std::vector<Shape2D> shapes;

    static struct Body2DTransform get_body_transform(const Body2D& body);
    static void set_body_transform(const struct Body2DTransform& transform);

    static void set_body_type(const Body2D& body, BodyType2D::Type type);
    static void set_body_gravity_scale(const Body2D& body, float scale);
    static void set_body_mass(const Body2D& body, float mass);
    static void set_body_size(const Body2D& body, const Vector3& size);
    static void set_body_radius(const Body2D& body, float radius);

    static void set_body_bounciness(const Body2D& body, float bounciness, ShapeType2D::Type type);
    static void set_body_friction(const Body2D& body, float friction, ShapeType2D::Type type);
};

/** 
 * @brief Structure representing the transform of a 2D physics body.
 * 
 * Includes position and rotation information.
 */
struct Body2DTransform
{
    Body2D body;
    Vector3 position;
    float rotation {0.0f};
};
