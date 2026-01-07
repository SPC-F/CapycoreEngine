#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/world/body/shape_type_2d.h>
#include <engine/physics/world/physics_world.h>

#include <stdexcept>

/* Very small base mass to avoid zero mass issues */
constexpr float base_mass = 0.001f;
constexpr float default_inertia = 0.0f;
constexpr float default_box_divisor = 2.0f;

PhysicsCreationFactory::PhysicsCreationFactory(b2WorldId world_id)
    : world_id_(world_id) {}

Body2D PhysicsCreationFactory::create_body(Vector3 position,
                                           BodyType2D::Type type,
                                           Component* component) {
  b2BodyDef body_def = b2DefaultBodyDef();

  switch (type) {
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

  body_def.position = b2Vec2{PhysicsMath::pixels_to_box2d(position.x),
                             PhysicsMath::pixels_to_box2d(position.y)};
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

Body2D PhysicsCreationFactory::create_box_fixture(Body2D body, Point offset,
                                                  float width, float height,
                                                  PhysicsCreationFlags flags) {
  if (width <= 0.0f || height <= 0.0f)
    throw std::invalid_argument("Width and height must be positive.");

  // Convert width/height from pixels to meters
  float width_m = PhysicsMath::pixels_to_box2d(width);
  float height_m = PhysicsMath::pixels_to_box2d(height);

  float half_width = width_m / 2.0f;
  float half_height = height_m / 2.0f;

  // Offset is only needed if you want the shape to be off-center
  b2Vec2 converted_offset = {PhysicsMath::pixels_to_box2d(offset.x),
                             PhysicsMath::pixels_to_box2d(offset.y)};

  // Make a Box2D box centered on the body
  b2Polygon box = b2MakeOffsetBox(half_width, half_height, converted_offset,
                                  b2MakeRot(0.0f));

  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.enableContactEvents = flags.enable_contact_events;
  shape_def.enableSensorEvents = flags.enable_sensor_events;
  shape_def.isSensor = flags.sensor;

  float density = 0.0f;

  if (flags.dynamic && flags.desired_mass > 0.0f) {
    float area = width_m * height_m;
    density = flags.desired_mass / area;
  }

  shape_def.density = density;

  b2ShapeId shape_id = b2CreatePolygonShape(body.id, &shape_def, &box);
  body.shapes.push_back({shape_id, b2ShapeType::b2_polygonShape});

  b2Filter filter{};
  filter.categoryBits = flags.category;
  filter.maskBits = flags.mask;
  b2Shape_SetFilter(shape_id, filter);
  b2Shape_SetFriction(shape_id, flags.friction);
  b2Shape_SetRestitution(shape_id, flags.bounciness);

  if (!b2Shape_IsValid(shape_id)) {
    throw std::runtime_error("Failed to create shape for body in Box2D world.");
  }

  b2Body_SetFixedRotation(body.id, !flags.enable_rotation);

  if (flags.enable_contact_events) {
    b2Shape_EnableContactEvents(shape_id, true);
  }

  if (flags.enable_sensor_events) {
    b2Shape_EnableSensorEvents(shape_id, true);
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

Body2D PhysicsCreationFactory::create_circle_fixture(
    Body2D body, Point offset, float radius, PhysicsCreationFlags flags) {
  if (radius <= 0.0f) {
    throw std::invalid_argument("Radius must be a positive value.");
  }

  b2Circle circle;
  circle.radius = PhysicsMath::pixels_to_box2d(radius);
  circle.center = b2Vec2{PhysicsMath::pixels_to_box2d(offset.x),
                         PhysicsMath::pixels_to_box2d(offset.y)};

  b2ShapeDef shape_def = b2DefaultShapeDef();

  shape_def.enableContactEvents = flags.enable_contact_events;
  shape_def.enableSensorEvents = flags.enable_sensor_events;
  shape_def.isSensor = flags.sensor;
  float density = 0.0f;

  if (flags.dynamic) {
    if (flags.desired_mass > 0.0f) {
      float area = B2_PI * circle.radius * circle.radius;
      density = flags.desired_mass / area;
    } else {
      density = 1.0f;
    }
  }

  shape_def.density = density;

  b2ShapeId shape_id = b2CreateCircleShape(body.id, &shape_def, &circle);

  body.shapes.push_back({shape_id, b2ShapeType::b2_circleShape});

  b2Filter filter{};
  filter.categoryBits = flags.category;
  filter.maskBits = flags.mask;
  b2Shape_SetFilter(shape_id, filter);
  b2Shape_SetFriction(shape_id, flags.friction);
  b2Shape_SetRestitution(shape_id, flags.bounciness);

  b2Body_SetFixedRotation(body.id, !flags.enable_rotation);

  if (!b2Shape_IsValid(shape_id)) {
    throw std::runtime_error("Failed to create shape for body in Box2D world.");
  }

  if (flags.enable_contact_events) {
    b2Shape_EnableContactEvents(shape_id, true);
  }

  if (flags.enable_sensor_events) {
    b2Shape_EnableSensorEvents(shape_id, true);
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

void PhysicsCreationFactory::destroy_body(Body2D& body) {
  b2DestroyBody(body.id);
  body.id = b2BodyId{};
  body.shapes.clear();
}