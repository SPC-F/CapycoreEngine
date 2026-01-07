#include <engine/physics/physics_math.h>
#include <engine/physics/world/body/body_2d.h>
#include <engine/physics/world/physics_world.h>

#include <iostream>

constexpr float force_multiplier = 100000.0f;
constexpr float default_box_divisor = 2.0f;

Body2DTransform Body2D::get_body_transform(const Body2D& body) {
  b2Vec2 pos = b2Body_GetPosition(body.id);
  b2Rot rotation = b2Body_GetRotation(body.id);
  float angle =
      PhysicsMath::convert_box2d_angle_to_radians(rotation.s, rotation.c);

  Body2DTransform transform{};
  transform.body = body;
  transform.position =
      PhysicsMath::box2d_vec_to_pixel_vec3({pos.x, pos.y, 0.0f}, false);
  transform.rotation = PhysicsMath::to_degrees(angle);

  b2Vec2 com = b2Body_GetWorldCenterOfMass(body.id);
  transform.center_of_mass =
      PhysicsMath::box2d_vec_to_pixel_vec3({com.x, com.y, 0.0f}, false);

  return transform;
}

Body2DTransform Body2D::get_pixel_transform(const Body2D& body) {
  b2Vec2 pos = b2Body_GetPosition(body.id);      // center of body
  b2Rot rotation = b2Body_GetRotation(body.id);  // radians

  Body2DTransform transform{};
  transform.body = body;

  // Convert Box2D position (meters) to pixels
  transform.position =
      PhysicsMath::box2d_vec_to_pixel_vec3({pos.x, pos.y, 0.0f});

  // Convert rotation to degrees for engine transform
  float angle_rad =
      PhysicsMath::convert_box2d_angle_to_radians(rotation.s, rotation.c);
  transform.rotation = PhysicsMath::to_degrees(angle_rad);

  // Center of mass (optional)
  b2Vec2 com = b2Body_GetWorldCenterOfMass(body.id);
  transform.center_of_mass =
      PhysicsMath::box2d_vec_to_pixel_vec3({com.x, com.y, 0.0f});

  return transform;
}

void Body2D::set_body_transform(const Body2DTransform& transform,
                                bool in_pixels) {
  Vector3 position =
      PhysicsMath::vec3_to_box2d_vec3(transform.position, in_pixels);
  b2Rot rotation{
      PhysicsMath::convert_radians_to_box2d_cosine(transform.rotation),
      PhysicsMath::convert_radians_to_box2d_sine(transform.rotation)};

  b2Body_SetTransform(transform.body.id, {position.x, position.y}, rotation);
}

void Body2D::set_body_type(const Body2D& body, BodyType2D::Type type) {
  b2BodyType b2_type = BodyType2D::to_box2d_body_type(type);
  b2Body_SetType(body.id, b2_type);
}

void Body2D::set_body_size(const Body2D& body, const Vector3& size,
                           Point offset) {
  float width_m = PhysicsMath::pixels_to_box2d(size.x);
  float height_m = PhysicsMath::pixels_to_box2d(size.y);

  float half_width = width_m / default_box_divisor;
  float half_height = height_m / default_box_divisor;

  b2Vec2 converted_offset = {
      PhysicsMath::pixels_to_box2d(offset.x + size.x * 0.5f),
      PhysicsMath::pixels_to_box2d(offset.y + size.y * 0.5f)};

  for (const auto& shape : body.shapes) {
    if (shape.type != b2ShapeType::b2_polygonShape) {
      continue;
    }

    b2Polygon box = b2MakeOffsetBox(half_width, half_height, converted_offset,
                                    b2MakeRot(0.0f));
    b2Shape_SetPolygon(shape.id, &box);
  }
}

void Body2D::set_body_radius(const Body2D& body, float radius, Point offset) {
  for (const auto& shape : body.shapes) {
    if (shape.type != b2ShapeType::b2_circleShape) {
      continue;
    }

    b2Circle circle;
    circle.radius = PhysicsMath::pixels_to_box2d(radius);
    circle.center = b2Vec2{PhysicsMath::pixels_to_box2d(offset.x),
                           PhysicsMath::pixels_to_box2d(offset.y)};

    b2Shape_SetCircle(shape.id, &circle);
  }
}

void Body2D::set_body_gravity_scale(const Body2D& body, float scale) {
  b2Body_SetGravityScale(body.id, scale);
}

void Body2D::set_body_mass(const Body2D& body, float mass) {
  b2MassData mass_data = b2Body_GetMassData(body.id);
  mass_data.mass = mass;

  b2Body_SetMassData(body.id, mass_data);
}

void Body2D::set_body_bounciness(const Body2D& body, float bounciness,
                                 ShapeType2D::Type type) {
  for (const auto& shape : body.shapes) {
    if (shape.type != ShapeType2D::to_box2d_type(type)) {
      continue;
    }

    b2Shape_SetRestitution(shape.id, bounciness);
  }
}

void Body2D::set_body_friction(const Body2D& body, float friction,
                               ShapeType2D::Type type) {
  for (const auto& shape : body.shapes) {
    if (shape.type != ShapeType2D::to_box2d_type(type)) {
      continue;
    }

    b2Shape_SetFriction(shape.id, friction);
  }
}

void Body2D::apply_force(const Body2D& body, const Vector3& force) noexcept {
  b2Vec2 b2_force{PhysicsMath::pixels_to_box2d(force.x),
                  PhysicsMath::pixels_to_box2d(force.y)};

  // Force multiplier is an unusually high value to compensate for Box2D's
  // physics calculations being tuned for smaller forces. This way we can apply
  // forces that feel more natural in a pixel-based environment.
  b2_force *= force_multiplier;

  b2Body_ApplyForceToCenter(body.id, b2_force, true);
}

void Body2D::apply_impulse(const Body2D& body,
                           const Vector3& impulse) noexcept {
  b2Vec2 b2_impulse{impulse.x, impulse.y};
  b2Vec2 local_center = b2Body_GetLocalCenterOfMass(body.id);
  b2Vec2 world_center = b2Body_GetWorldPoint(body.id, local_center);

  b2Body_ApplyLinearImpulse(body.id, b2_impulse, world_center, true);
}

void Body2D::set_body_velocity(const Body2D& body,
                               const Vector3& velocity) noexcept {
  b2Vec2 b2_velocity{velocity.x, velocity.y};
  b2Body_SetLinearVelocity(body.id, b2_velocity);
}

Vector3 Body2D::get_body_velocity(const Body2D& body) noexcept {
  b2Vec2 vel = b2Body_GetLinearVelocity(body.id);
  return Vector3{vel.x, vel.y, 0.0f};
}

float Body2D::get_body_angular_velocity(const Body2D& body) noexcept {
  return b2Body_GetAngularVelocity(body.id);
}

void Body2D::set_body_angular_velocity(const Body2D& body,
                                       float angular_velocity) noexcept {
  b2Body_SetAngularVelocity(body.id, angular_velocity);
}

PolygonVerts Body2D::get_body_polygon_verts(const Body2D& body,
                                            b2ShapeId shape_id) {
  PolygonVerts verts{};
  b2Polygon poly = b2Shape_GetPolygon(shape_id);
  b2Transform xf = b2Body_GetTransform(body.id);

  int n = (poly.count < MAX_POLY_VERTS) ? poly.count : MAX_POLY_VERTS;
  verts.count = n;

  for (int i = 0; i < n; i++) {
    b2Vec2 worldV = b2TransformPoint(xf, poly.vertices[i]);
    float px = PhysicsMath::box2d_to_pixels(worldV.x);
    float py = PhysicsMath::box2d_to_pixels(worldV.y);
    verts.verts[i] = {px, py};
  }

  return verts;
}

Vec2f Body2D::get_body_world_position(const Body2D& body) {
  b2Transform xf = b2Body_GetTransform(body.id);
  b2Vec2 pos = xf.p;

  return {PhysicsMath::box2d_to_pixels(pos.x),
          PhysicsMath::box2d_to_pixels(pos.y)};
}

float Body2D::get_body_world_rotation(const Body2D& body) {
  b2Transform xf = b2Body_GetTransform(body.id);

  return PhysicsMath::to_degrees(
      PhysicsMath::convert_box2d_angle_to_radians(xf.q.s, xf.q.c));
}