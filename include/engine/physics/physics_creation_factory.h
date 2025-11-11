#pragma once

#include <box2d/box2d.h>
#include <memory>

#include <engine/public/component.h>
#include <engine/public/util/point.h>

/**
 * @brief Flags for creating physics bodies.
 * 
 * This struct encapsulates various options that can be set
 * when creating physics bodies in the Box2D world.
 */
struct PhysicsCreationFlags
{
    bool dynamic{true};
    bool sensor{false};
    bool enable_contact_events{true};
    bool is_bullet{false};
};

/**
 * @brief Factory class for creating physics bodies in the Box2D world.
 * 
 * This class provides methods to create various types of physics bodies
 * (such as boxes and circles) with specified properties and flags.
 */
class PhysicsCreationFactory {
public:
    PhysicsCreationFactory(b2WorldId world_id);

    /** @brief Create a box body with specified parameters and flags */
    b2BodyId create_box_body(Point position, float width, float height, PhysicsCreationFlags flags, Component* component);
    /** @brief Create a circle body with specified parameters and flags */
    b2BodyId create_circle_body(Point position, float radius, PhysicsCreationFlags flags, Component* component);

    /** @brief Destroy a body given its identifier */
    static void destroy_body(b2BodyId body_id);

private:
    b2WorldId& world_id_;
};