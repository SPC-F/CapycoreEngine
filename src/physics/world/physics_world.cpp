#include <engine/physics/physics_math.h>
#include <engine/physics/world/physics_world.h>
#include <engine/public/components/colliders/box_collider_2d.h>
#include <engine/public/components/colliders/circle_collider_2d.h>
#include <engine/public/components/colliders/collider_2d.h>
#include <engine/public/gameObject.h>

#include <stdexcept>

PhysicsWorld::PhysicsWorld() {
  b2WorldDef world_def = b2DefaultWorldDef();
  world_def.gravity = b2Vec2{gravity_x_, gravity_y_};
  world_id_ = b2CreateWorld(&world_def);

  if (!b2World_IsValid(world_id_)) {
    throw std::runtime_error("Failed to create Box2D world.");
  }
}

PhysicsWorld::~PhysicsWorld() { b2DestroyWorld(world_id_); }

b2WorldId PhysicsWorld::world_id() const noexcept { return world_id_; }

void PhysicsWorld::step(float dt) {
  b2World_Step(world_id_, dt, velocity_iterations_);
}

void PhysicsWorld::check_collision(
    const std::vector<std::reference_wrapper<GameObject>>& objects) {
  auto find_collider_in_objects = [&](Component* comp, b2ShapeId shape_id)
      -> std::optional<std::reference_wrapper<Collider2D>> {
    for (const auto& obj_ref : objects) {
      GameObject& obj = obj_ref.get();
      auto collider_opts = obj.get_components<Collider2D>();

      for (auto& collider_opt : collider_opts) {
        Collider2D& collider = collider_opt.get();
        auto shapes = collider.get_rigidbody().get().body().shapes;

        for (const auto& shape : shapes) {
          if (shape.id.index1 == shape_id.index1 &&
              shape.id.generation == shape_id.generation) {
            // Match shape type
            b2ShapeType collider_shape_type;
            if (dynamic_cast<BoxCollider2D*>(&collider)) {
              collider_shape_type = b2ShapeType::b2_polygonShape;
            } else if (dynamic_cast<CircleCollider2D*>(&collider)) {
              collider_shape_type = b2ShapeType::b2_circleShape;
            } else {
              continue;
            }

            if (collider.parent()->get().id() == comp->parent()->get().id() &&
                collider_shape_type == shape.type) {
              return std::ref(collider);
            }
          }
        }
      }
    }
    return std::nullopt;
  };

  /// Check contact (collision) events
  b2ContactEvents contact_events = b2World_GetContactEvents(world_id_);
  for (int i = 0; i < contact_events.beginCount; ++i) {
    b2ContactBeginTouchEvent* touch_event = contact_events.beginEvents + i;

    if (!b2Shape_IsValid(touch_event->shapeIdA) ||
        !b2Shape_IsValid(touch_event->shapeIdB))
      continue;

    b2BodyId body_a = b2Shape_GetBody(touch_event->shapeIdA);
    b2BodyId body_b = b2Shape_GetBody(touch_event->shapeIdB);
    auto* comp_a = static_cast<Component*>(b2Body_GetUserData(body_a));
    auto* comp_b = static_cast<Component*>(b2Body_GetUserData(body_b));

    if (!comp_a || !comp_b) continue;

    if (auto collider_a_opt =
            find_collider_in_objects(comp_a, touch_event->shapeIdA);
        collider_a_opt.has_value()) {
      if (auto collider_b_opt =
              find_collider_in_objects(comp_b, touch_event->shapeIdB);
          collider_b_opt.has_value()) {
        Collider2D& colA = collider_a_opt->get();
        Collider2D& colB = collider_b_opt->get();

        if (colA.active()) colA.on_collision_enter(colB);
        if (colB.active()) colB.on_collision_enter(colA);
      }
    }
  }

  /// Check sensor (trigger) events
  b2SensorEvents sensor_events = b2World_GetSensorEvents(world_id_);
  for (int i = 0; i < sensor_events.beginCount; ++i) {
    b2SensorBeginTouchEvent* touch_event = sensor_events.beginEvents + i;

    if (!b2Shape_IsValid(touch_event->sensorShapeId) ||
        !b2Shape_IsValid(touch_event->visitorShapeId))
      continue;

    b2BodyId body_a = b2Shape_GetBody(touch_event->sensorShapeId);
    b2BodyId body_b = b2Shape_GetBody(touch_event->visitorShapeId);
    auto* comp_a = static_cast<Component*>(b2Body_GetUserData(body_a));
    auto* comp_b = static_cast<Component*>(b2Body_GetUserData(body_b));

    if (!comp_a || !comp_b) continue;

    if (auto collider_a_opt =
            find_collider_in_objects(comp_a, touch_event->sensorShapeId);
        collider_a_opt.has_value()) {
      if (auto collider_b_opt =
              find_collider_in_objects(comp_b, touch_event->visitorShapeId);
          collider_b_opt.has_value()) {
        Collider2D& colA = collider_a_opt->get();
        Collider2D& colB = collider_b_opt->get();

        if (colA.creation_flags().sensor && colA.active())
          colA.on_trigger_enter(colB);
        if (colB.creation_flags().sensor && colB.active())
          colB.on_trigger_enter(colA);
      }
    }
  }
}

BodyDistance2D PhysicsWorld::distance(const Body2DTransform& a,
                                      const Body2DTransform& b) {
  Point normal{0.0f, 0.0f};
  Point point_a{a.position.x, a.position.y};
  Point point_b{b.position.x, b.position.y};

  float length = PhysicsMath::distance_between_points(point_a, point_b);
  float dx = PhysicsMath::dx_between_points(point_a, point_b);
  float dy = PhysicsMath::dy_between_points(point_a, point_b);

  if (length != 0.0f) {
    normal.x = dx / length;
    normal.y = dy / length;
  }

  BodyDistance2D result{};
  result.distance = length;
  result.normal = normal;
  result.point_a = point_a;
  result.point_b = point_b;

  return result;
}

BodyDistance2D PhysicsWorld::fixture_distance(const Body2DTransform& a,
                                              const Body2DTransform& b) {
  BodyDistance2D result{};
  result.distance = FLT_MAX;  // minimum distance search
  result.normal = Point{0.0f, 0.0f};

  b2SimplexCache cache{};
  cache.count = 0;

  b2Transform b2_transform_a{
      b2Vec2{a.position.x, a.position.y},
      b2Rot{PhysicsMath::convert_radians_to_box2d_cosine(a.rotation),
            PhysicsMath::convert_radians_to_box2d_sine(a.rotation)}};

  b2Transform b2_transform_b{
      b2Vec2{b.position.x, b.position.y},
      b2Rot{PhysicsMath::convert_radians_to_box2d_cosine(b.rotation),
            PhysicsMath::convert_radians_to_box2d_sine(b.rotation)}};

  auto create_shape_proxy = [](b2ShapeId shape_id) -> b2ShapeProxy {
    b2ShapeProxy proxy{};

    auto type = b2Shape_GetType(shape_id);
    if (type == b2ShapeType::b2_circleShape) {
      b2Circle circle = b2Shape_GetCircle(shape_id);

      proxy.count = 1;
      proxy.radius = circle.radius;
      proxy.points[0] = circle.center;  // local center
    } else if (type == b2ShapeType::b2_polygonShape) {
      b2Polygon poly = b2Shape_GetPolygon(shape_id);

      proxy.count = poly.count;
      proxy.radius = poly.radius;

      for (int i = 0; i < poly.count; ++i) {
        proxy.points[i] = poly.vertices[i];
      }
    }

    return proxy;
  };

  for (const auto& shape_a : a.body.shapes) {
    b2ShapeProxy proxy_a = create_shape_proxy(shape_a.id);

    for (const auto& shape_b : b.body.shapes) {
      b2ShapeProxy proxy_b = create_shape_proxy(shape_b.id);

      b2DistanceInput input{};
      input.proxyA = proxy_a;
      input.proxyB = proxy_b;
      input.transformA = b2_transform_a;
      input.transformB = b2_transform_b;
      input.useRadii = true;

      b2DistanceOutput output = b2ShapeDistance(&input, &cache, nullptr, 0);

      if (output.distance < result.distance) {
        result.distance = output.distance;
        result.point_a = Point{output.pointA.x, output.pointA.y};
        result.point_b = Point{output.pointB.x, output.pointB.y};
        result.normal = Point{output.normal.x, output.normal.y};
      }
    }
  }

  return result;
}

int32_t PhysicsWorld::velocity_iterations() const noexcept {
  return velocity_iterations_;
}

int32_t PhysicsWorld::velocity_iterations(int32_t iterations) noexcept {
  velocity_iterations_ = iterations;
  return velocity_iterations_;
}

float PhysicsWorld::gravity_x() const noexcept { return gravity_x_; }

float PhysicsWorld::gravity_x(float gx) noexcept {
  gravity_x_ = gx;

  if (world_exists()) {
    b2World_SetGravity(world_id_, b2Vec2{gravity_x_, gravity_y_});
  }

  return gravity_x_;
}

float PhysicsWorld::gravity_y() const noexcept { return gravity_y_; }

float PhysicsWorld::gravity_y(float gy) noexcept {
  gravity_y_ = gy;

  if (world_exists()) {
    b2World_SetGravity(world_id_, b2Vec2{gravity_x_, gravity_y_});
  }

  return gravity_y_;
}

bool PhysicsWorld::world_exists() const noexcept {
  return world_id_.index1 != 0 || world_id_.generation != 0;
}