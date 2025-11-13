#include <engine/physics/physics_creation_factory.h>

/* Very small base mass to avoid zero mass issues */
constexpr float base_mass = 0.000001f;
constexpr float default_inertia = 0.0f;
constexpr float default_box_divisor = 2.0f;

PhysicsCreationFactory::PhysicsCreationFactory(b2WorldId world_id)
    : world_id_(world_id) {}

b2BodyId PhysicsCreationFactory::create_body(Point position, b2BodyType type, Component* component) {
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = type;
    body_def.position = b2Vec2{position.x, position.y};
    body_def.userData = component;
    
    b2BodyId body_id = b2CreateBody(world_id_, &body_def);
    if (!b2Body_IsValid(body_id)) {
        throw std::runtime_error("Failed to create body in Box2D world.");
    }

    return body_id;
}

b2BodyId PhysicsCreationFactory::create_box_fixture(b2BodyId body, float width, float height, PhysicsCreationFlags flags) {
    if (width <= 0.0f || height <= 0.0f)
    {
        throw std::invalid_argument("Width and height must be positive values.");
    }

    b2Polygon box = b2MakeBox(width / default_box_divisor, height / default_box_divisor);
    
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.enableContactEvents = flags.enable_contact_events;
    shape_def.isSensor = flags.sensor;

    b2ShapeId shape_id = b2CreatePolygonShape(body, &shape_def, &box);
    
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

        b2Body_SetMassData(body, mass_data);
        b2Body_SetBullet(body, true);
    }

    return body;
}

b2BodyId PhysicsCreationFactory::create_circle_fixture(b2BodyId body, float radius, PhysicsCreationFlags flags) {
    if (radius <= 0.0f)
    {
        throw std::invalid_argument("Radius must be a positive value.");
    }

    b2Circle circle;
    circle.center = b2Vec2{0.0f, 0.0f};
    circle.radius = radius;

    b2ShapeDef shape_def = b2DefaultShapeDef();

    shape_def.enableContactEvents = flags.enable_contact_events;
    shape_def.isSensor = flags.sensor;

    b2ShapeId shape_id = b2CreateCircleShape(body, &shape_def, &circle);

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

        b2Body_SetMassData(body, mass_data);
        b2Body_SetBullet(body, true);
    }

    return body;
}

void PhysicsCreationFactory::destroy_body(b2BodyId body_id) {
    b2DestroyBody(body_id);
}