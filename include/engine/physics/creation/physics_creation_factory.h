#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <vector>

#include <engine/physics/creation/physics_creation_flags.h>
#include <engine/physics/world/body/body_type_2d.h>
#include <engine/physics/world/body/body_2d.h>
#include <engine/public/component.h>
#include <engine/public/util/vector3.h>

/**
 * @brief Factory class for creating physics bodies in the Box2D world.
 * 
 * This class provides methods to create various types of physics bodies
 * (such as boxes and circles) with specified properties and flags.
 */
class PhysicsCreationFactory {
public:
    PhysicsCreationFactory(b2WorldId world_id);

    /**
     * @brief Create a physics body in the Box2D world.
     * 
     * @param position The initial position of the body
     * @param type The type of the body (static, dynamic, kinematic)
     * @param component The component associated with the body
     * @return b2BodyId The identifier of the created body
     */
    Body2D create_body(Vector3 position, BodyType2D::Type type, Component* component);
    
    /**
     * @brief Create a box-shaped fixture for a given body.
     * 
     * @param body The identifier of the body to attach the fixture to
     * @param width The width of the box
     * @param height The height of the box
     * @param flags The creation flags for the fixture
     * @return b2BodyId The identifier of the created fixture
     */
    static Body2D create_box_fixture(Body2D body, float width, float height, PhysicsCreationFlags flags);

    /**
     * @brief Create a circle-shaped fixture for a given body.
     * 
     * @param body The identifier of the body to attach the fixture to
     * @param radius The radius of the circle
     * @param flags The creation flags for the fixture
     * @param component The component associated with the fixture
     * @return b2BodyId The identifier of the created fixture
     */
    static Body2D create_circle_fixture(Body2D body, float radius, PhysicsCreationFlags flags);

    /**
     * @brief Destroy a body given its identifier
     * 
     * @param body_id The identifier of the body to destroy
     */
    static void destroy_body(Body2D& body);

private:
    b2WorldId world_id_;
};