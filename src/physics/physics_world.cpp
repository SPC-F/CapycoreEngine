#include <engine/physics/physics_world.h>
#include <engine/public/gameObject.h>

PhysicsWorld::PhysicsWorld() {
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = b2Vec2{gravity_x_, gravity_y_};
    world_id_ = b2CreateWorld(&world_def);

    if (!b2World_IsValid(world_id_)) {
        throw std::runtime_error("Failed to create Box2D world.");
    }
}

PhysicsWorld::~PhysicsWorld() {
    b2DestroyWorld(world_id_);
}

b2WorldId PhysicsWorld::world_id() const noexcept {
    return world_id_;
}

void PhysicsWorld::step() {
    b2World_Step(world_id_, time_step_, velocity_iterations_);
    check_collision();
}

void PhysicsWorld::check_collision() {
    b2ContactEvents contact_events = b2World_GetContactEvents(world_id_);
    for (int i = 0; i < contact_events.beginCount; ++i)
    {
        b2ContactBeginTouchEvent* touch_event = contact_events.beginEvents + i;

        if (!b2Shape_IsValid(touch_event->shapeIdA) ||
            !b2Shape_IsValid(touch_event->shapeIdB))
        {
            continue;
        }

        // TODO: add after component is created
        // b2BodyId body_a = b2Shape_GetBody(touch_event->shapeIdA);
        // b2BodyId body_b = b2Shape_GetBody(touch_event->shapeIdB);
        // auto* game_object_a = static_cast<Component*>(b2Body_GetUserData(body_a));
        // auto* game_object_b = static_cast<Component*>(b2Body_GetUserData(body_b));

    }
}

float PhysicsWorld::time_step() const noexcept {
    return time_step_;
}

float PhysicsWorld::time_step(float timestep) noexcept {
    time_step_ = timestep;
    return time_step_;
}

int32_t PhysicsWorld::velocity_iterations() const noexcept {
    return velocity_iterations_;
}

int32_t PhysicsWorld::velocity_iterations(int32_t iterations) noexcept {
    velocity_iterations_ = iterations;
    return velocity_iterations_;
}

float PhysicsWorld::gravity_x() const noexcept {
    return gravity_x_;
}

float PhysicsWorld::gravity_x(float gx) noexcept {
    gravity_x_ = gx;

    if (world_exists()) {
        b2World_SetGravity(world_id_, b2Vec2{gravity_x_, gravity_y_});
    }

    return gravity_x_;
}

float PhysicsWorld::gravity_y() const noexcept {
    return gravity_y_;
}

float PhysicsWorld::gravity_y(float gy) noexcept {
    gravity_y_ = gy;

    if (world_exists()) {
        b2World_SetGravity(world_id_, b2Vec2{gravity_x_, gravity_y_});
    }

    return gravity_y_;
}

float PhysicsWorld::pixel_to_meter_ratio() const noexcept {
    return pixel_to_meter_ratio_;
}

float PhysicsWorld::pixel_to_meter_ratio(float ratio) noexcept {
    pixel_to_meter_ratio_ = ratio;
    return pixel_to_meter_ratio_;
}

bool PhysicsWorld::world_exists() const noexcept {
    return world_id_.index1 != 0 || world_id_.generation != 0;
}