#include <engine/physics/creation/physics_creation_factory.h>

#include <stdexcept>
#include <engine/physics/world/body/shape_type_2d.h>

/* Very small base mass to avoid zero mass issues */
constexpr float base_mass = 0.000001f;
constexpr float default_inertia = 0.0f;
constexpr float default_box_divisor = 2.0f;

PhysicsCreationFactory::PhysicsCreationFactory(b2WorldId world_id)
    : world_id_(world_id) {}

Body2D PhysicsCreationFactory::create_body(Vector3 position, BodyType2D::Type type, Component* component) 
{
    b2BodyDef body_def = b2DefaultBodyDef();

    switch (type)
    {
        case BodyType2D::Static:
            body_def.type = b2_staticBody;
            break;
        case BodyType2D::Kinematic:
            body_def.type = b2_kinematicBody;
            break;
        case BodyType2D::Dynamic:
        default:
            body_def.type = b2_dynamicBody;
            break;
    }

    body_def.position = b2Vec2{position.x, position.y};
    body_def.userData = component;
    
    b2BodyId body_id = b2CreateBody(world_id_, &body_def);
    if (!b2Body_IsValid(body_id)) {
        throw std::runtime_error("Failed to create body in Box2D world.");
    }

    Body2D body;
    body.id = body_id;
    body.shapes = {};

    return body;
}

Body2D PhysicsCreationFactory::create_box_fixture(Body2D body, Point offset, float width, float height, PhysicsCreationFlags flags) 
{
    if (width <= 0.0f || height <= 0.0f)
    {
        throw std::invalid_argument("Width and height must be positive values.");
    }

    b2Polygon box = b2MakeOffsetBox(width / default_box_divisor, height / default_box_divisor, b2Vec2{offset.x, offset.y}, b2MakeRot(0.0f));
    
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.enableContactEvents = flags.enable_contact_events;
    shape_def.isSensor = flags.sensor;

    b2ShapeId shape_id = b2CreatePolygonShape(body.id, &shape_def, &box);
    body.shapes.push_back({ shape_id, b2ShapeType::b2_polygonShape });
    
    b2Filter filter{};
    filter.categoryBits = flags.category;
    filter.maskBits = flags.mask;
    b2Shape_SetFilter(shape_id, filter);

    if (!b2Shape_IsValid(shape_id)) {
        throw std::runtime_error("Failed to create shape for body in Box2D world.");
    }

    if (flags.enable_contact_events) {
        b2Shape_EnableContactEvents(shape_id, true);
    }

    if (flags.is_bullet) {
        b2MassData mass_data;
        mass_data.mass = base_mass;
        mass_data.center = b2Vec2{0.0f, 0.0f};
        mass_data.rotationalInertia = default_inertia;

        b2Body_SetMassData(body.id, mass_data);
        b2Body_SetBullet(body.id, true);
    }

    return body;
}

Body2D PhysicsCreationFactory::create_circle_fixture(Body2D body, Point offset, float radius, PhysicsCreationFlags flags) 
{
    if (radius <= 0.0f)
    {
        throw std::invalid_argument("Radius must be a positive value.");
    }

    b2Circle circle;
    circle.radius = radius;
    circle.center = b2Vec2{offset.x, offset.y};

    b2ShapeDef shape_def = b2DefaultShapeDef();

    shape_def.enableContactEvents = flags.enable_contact_events;
    shape_def.isSensor = flags.sensor;

    b2ShapeId shape_id = b2CreateCircleShape(body.id, &shape_def, &circle);
    
    body.shapes.push_back({ shape_id, b2ShapeType::b2_circleShape });

    b2Filter filter{};
    filter.categoryBits = flags.category;
    filter.maskBits = flags.mask;
    b2Shape_SetFilter(shape_id, filter);
    
    if (!b2Shape_IsValid(shape_id)) {
        throw std::runtime_error("Failed to create shape for body in Box2D world.");
    }

    if (flags.enable_contact_events) {
        b2Shape_EnableContactEvents(shape_id, true);
    }

    if (flags.is_bullet) {
        b2MassData mass_data;
        mass_data.mass = base_mass;
        mass_data.center = b2Vec2{0.0f, 0.0f};
        mass_data.rotationalInertia = default_inertia;

        b2Body_SetMassData(body.id, mass_data);
        b2Body_SetBullet(body.id, true);
    }

    return body;
}

void PhysicsCreationFactory::destroy_body(Body2D& body) 
{
    b2DestroyBody(body.id);
    body.id = b2BodyId{};
    body.shapes.clear();
}