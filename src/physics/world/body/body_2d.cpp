#include <engine/physics/world/body/body_2d.h>

#include <engine/physics/physics_math.h>

constexpr float default_box_divisor = 2.0f;

Body2DTransform Body2D::get_body_transform(const Body2D& body) 
{
    b2Vec2 pos = b2Body_GetPosition(body.id);
    b2Rot rotation = b2Body_GetRotation(body.id);
    float angle = PhysicsMath::convert_box2d_angle_to_radians(rotation.s, rotation.c);

    Body2DTransform transform{};
    transform.body = body;
    transform.position = Vector3{pos.x, pos.y, 0.0f};
    transform.rotation = angle;

    return transform;
}

void Body2D::set_body_transform(const Body2DTransform& transform) 
{
    b2Vec2 position {transform.position.x, transform.position.y};
    b2Rot rotation {PhysicsMath::convert_radians_to_box2d_cosine(transform.rotation),
                    PhysicsMath::convert_radians_to_box2d_sine(transform.rotation)};

    b2Body_SetTransform(transform.body.id, position, rotation);
}

void Body2D::set_body_type(const Body2D& body, BodyType2D::Type type) 
{
    b2BodyType b2_type = BodyType2D::to_box2d_body_type(type);
    b2Body_SetType(body.id, b2_type);
}


void Body2D::set_body_size(const Body2D& body, const Vector3& size) 
{
    for (const auto& shape : body.shapes) {
        if (shape.type != b2ShapeType::b2_polygonShape) {
            continue;
        }

        b2Polygon box = b2MakeBox(size.x / default_box_divisor, size.y / default_box_divisor);
        b2Shape_SetPolygon(shape.id, &box);
    } 
}


void Body2D::set_body_radius(const Body2D& body, float radius) 
{
    for (const auto& shape : body.shapes) {
        if (shape.type != b2ShapeType::b2_circleShape) {
            continue;
        }

        b2Circle circle;
            
        circle.center = b2Vec2{0.0f, 0.0f};
        circle.radius = radius;

        b2Shape_SetCircle(shape.id, &circle);
    }
}

void Body2D::set_body_gravity_scale(const Body2D& body, float scale) 
{
    b2Body_SetGravityScale(body.id, scale);
}

void Body2D::set_body_mass(const Body2D& body, float mass) 
{
    b2MassData mass_data = b2Body_GetMassData(body.id);
    mass_data.mass = mass;
    
    b2Body_SetMassData(body.id, mass_data);
}

void Body2D::set_body_bounciness(const Body2D& body, float bounciness, ShapeType2D::Type type) 
{
    for (const auto& shape : body.shapes) {
        if (shape.type != ShapeType2D::to_box2d_type(type)) {
            continue;
        }

        b2Shape_SetRestitution(shape.id, bounciness);
    }
}

void Body2D::set_body_friction(const Body2D& body, float friction, ShapeType2D::Type type) 
{
    for (const auto& shape : body.shapes) {
        if (shape.type != ShapeType2D::to_box2d_type(type)) {
            continue;
        }
        
        b2Shape_SetFriction(shape.id, friction);
    }
}

void Body2D::apply_force(const Body2D& body, const Vector3& force) noexcept 
{
    b2Vec2 b2_force {force.x, force.y};
    b2Vec2 local_center = b2Body_GetLocalCenterOfMass(body.id);
    b2Vec2 world_center = b2Body_GetWorldPoint(body.id, local_center);

    b2Body_ApplyForce(body.id, b2_force, world_center, true);
}

void Body2D::apply_impulse(const Body2D& body, const Vector3& impulse) noexcept 
{
    b2Vec2 b2_impulse {impulse.x, impulse.y};
    b2Vec2 local_center = b2Body_GetLocalCenterOfMass(body.id);
    b2Vec2 world_center = b2Body_GetWorldPoint(body.id, local_center);

    b2Body_ApplyLinearImpulse(body.id, b2_impulse, world_center, true);
}

void Body2D::set_body_velocity(const Body2D& body, const Vector3& velocity) noexcept 
{
    b2Vec2 b2_velocity {velocity.x, velocity.y};
    b2Body_SetLinearVelocity(body.id, b2_velocity);
}

Vector3 Body2D::get_body_velocity(const Body2D& body) noexcept 
{
    b2Vec2 vel = b2Body_GetLinearVelocity(body.id);
    return Vector3{vel.x, vel.y, 0.0f};
}