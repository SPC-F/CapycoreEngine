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

    /**
     * @brief Retrieves the transform of the specified body.
     * 
     * @param body The Body2D instance to get the transform for.
     * @return Body2DTransform The transform of the body.
     */
    static struct Body2DTransform get_body_transform(const Body2D& body);

    /**
     * @brief Sets the transform of the specified body.
     * 
     * @param transform The Body2DTransform instance containing the new transform data.
     */
    static void set_body_transform(const struct Body2DTransform& transform);

    /** @brief Sets the type of the specified body.
     * 
     * @param body The Body2D instance to set the type for.
     * @param type The BodyType2D::Type to set.
     */
    static void set_body_type(const Body2D& body, BodyType2D::Type type);

    /** @brief Sets the size of the specified body.
     * 
     * @param body The Body2D instance to set the size for.
     * @param size The new size as a Vector3.
     */
    static void set_body_gravity_scale(const Body2D& body, float scale);

    /** @brief Sets the mass of the specified body.
     * 
     * @param body The Body2D instance to set the mass for.
     * @param mass The new mass value.
     */
    static void set_body_mass(const Body2D& body, float mass);

    /** @brief Sets the size of the specified body.
     * 
     * @param body The Body2D instance to set the size for.
     * @param size The new size as a Vector3.
     */
    static void set_body_size(const Body2D& body, const Vector3& size);

    /** @brief Sets the radius of the specified body.
     * 
     * @param body The Body2D instance to set the radius for.
     * @param radius The new radius value.
     */
    static void set_body_radius(const Body2D& body, float radius);

    /** @brief Sets the bounciness of the specified body for a given shape type.
     * 
     * @param body The Body2D instance to set the bounciness for.
     * @param bounciness The new bounciness value.
     * @param type The ShapeType2D::Type to apply the bounciness to.
     */
    static void set_body_bounciness(const Body2D& body, float bounciness, ShapeType2D::Type type);

    /** @brief Sets the friction of the specified body for a given shape type.
     * 
     * @param body The Body2D instance to set the friction for.
     * @param friction The new friction value.
     * @param type The ShapeType2D::Type to apply the friction to.
     */
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
