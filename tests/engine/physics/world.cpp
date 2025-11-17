#include <catch2/catch_test_macros.hpp>

#include <engine/physics/world/physics_world.h>
#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/public/util/vector3.h>
#include <engine/physics/physics_math.h>

TEST_CASE("physics_world_creates_valid_world", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld physics_world;

    // act & assert
    REQUIRE(b2World_IsValid(physics_world.world_id()));
}

TEST_CASE("physics_world_sets_and_gets_gravity", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act
    world.gravity_x(0.0f);
    world.gravity_y(-9.81f);

    // assert
    REQUIRE(world.gravity_x() == 0.0f);
    REQUIRE(world.gravity_y() == -9.81f);
}

TEST_CASE("physics_world_sets_and_gets_velocity_iterations", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act
    world.velocity_iterations(12);

    // assert
    REQUIRE(world.velocity_iterations() == 12);
}

TEST_CASE("physics_world_updates_gravity_when_world_exists", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;
    REQUIRE(world.world_exists());

    // act
    world.gravity_x(0.0f);
    world.gravity_y(-10.0f);

    b2Vec2 gravity = b2World_GetGravity(world.world_id());

    REQUIRE(gravity.x == 0.0f);
    REQUIRE(gravity.y == -10.0f);
}

TEST_CASE("physics_world_sets_and_gets_pixel_to_meter_ratio", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act
    world.pixel_to_meter_ratio(100.0f);

    // assert
    REQUIRE(world.pixel_to_meter_ratio() == 100.0f);
}

TEST_CASE("physics_world_step_and_check_collision", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act & assert
    REQUIRE_NOTHROW(world.step(1.0f / 60.0f));
    REQUIRE_NOTHROW(world.check_collision({}));
}

TEST_CASE("physics_world_step_updates_dynamic_body", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 position{0.0f, 10.0f, 0.0f};
    Body2D body = factory.create_body(position, BodyType2D::Dynamic, nullptr);
    body = factory.create_box_fixture(body, 1.0f, 1.0f, flags);
    b2Vec2 initial_pos = b2Body_GetPosition(body.id);

    // act
    int steps = 5; // step ~5 frames
    for (int i = 0; i < steps; ++i) {
        world.step(1.0f / 60.0f);
    }
    b2Vec2 new_pos = b2Body_GetPosition(body.id);

    // assert
    REQUIRE(new_pos.y < initial_pos.y + 0.1f); // allow small vertical drift
    REQUIRE(new_pos.x < initial_pos.x + 0.1f); // allow small horizontal drift
    REQUIRE(new_pos.x > initial_pos.x - 0.1f);

    REQUIRE_NOTHROW(world.check_collision({}));
}


TEST_CASE("physics_world_world_exists_returns_false_for_invalid_world", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act
    b2WorldId invalid_world_id{};
    PhysicsWorld invalid_world;
    invalid_world.gravity_x(0.0f);

    // assert
    REQUIRE(invalid_world.world_exists() == true);
}

TEST_CASE("physics_world_world_exists_returns_true_for_valid_world", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    // act & assert
    REQUIRE(world.world_exists() == true);
}

TEST_CASE("physics_world_gets_body_transform", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 position{5.0f, 15.0f, 0.0f};
    Body2D body = factory.create_body(position, BodyType2D::Static, nullptr);
    body = factory.create_box_fixture(body, 1.0f, 1.0f, flags);

    // act
    Body2DTransform transform = Body2D::get_body_transform(body);

    // assert
    REQUIRE(transform.position.x == 5.0f);
    REQUIRE(transform.position.y == 15.0f);
}

TEST_CASE("physics_world_sets_body_transform", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 initial_position{0.0f, 0.0f, 0.0f};
    Body2D body = factory.create_body(initial_position, BodyType2D::Static, nullptr);
    body = factory.create_box_fixture(body, 1.0f, 1.0f, flags);

    Vector3 new_position{10.0f, 20.0f, 0.0f};
    float new_rotation = PhysicsMath::to_radians(45.0f); // 45 degrees in radians

    // act
    Body2DTransform transform{body, new_position, new_rotation};
    Body2D::set_body_transform(transform);

    Body2DTransform updated_transform = Body2D::get_body_transform(body);

    // assert
    REQUIRE(updated_transform.position.x == 10.0f);
    REQUIRE(updated_transform.position.y == 20.0f);
    REQUIRE(updated_transform.rotation == new_rotation);
}

TEST_CASE("physics_world_destroys_body", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 position{0.0f, 0.0f, 0.0f};
    Body2D body = factory.create_body(position, BodyType2D::Static, nullptr);
    body = factory.create_box_fixture(body, 1.0f, 1.0f, flags);

    // act
    REQUIRE(b2Body_IsValid(body.id) == true);
    factory.destroy_body(body);
    
    // assert
    REQUIRE(b2Body_IsValid(body.id) == false);
}

TEST_CASE("physics_world_calculates_distance_between_bodies", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 position_a{0.0f, 0.0f, 0.0f};
    Body2D body_a = factory.create_body(position_a, BodyType2D::Static, nullptr);
    body_a = factory.create_box_fixture(body_a, 1.0f, 1.0f, flags);

    Vector3 position_b{5.0f, 0.0f, 0.0f};
    Body2D body_b = factory.create_body(position_b, BodyType2D::Static, nullptr);
    body_b = factory.create_box_fixture(body_b, 1.0f, 1.0f, flags);

    Body2DTransform transform_a = Body2D::get_body_transform(body_a);
    Body2DTransform transform_b = Body2D::get_body_transform(body_b);

    // act
    BodyDistance2D distance_info = world.distance(transform_a, transform_b);

    // assert
    REQUIRE(distance_info.distance == 5.0f);
    REQUIRE(distance_info.distance > 0.0f);
    REQUIRE(distance_info.normal.x != 0.0f);
    REQUIRE(distance_info.normal.y == 0.0f);
}

TEST_CASE("physics_world_calculates_fixture_distance_between_bodies", "[PhysicsWorld]") {
    // arrange
    PhysicsWorld world;

    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    Vector3 position_a{0.0f, 0.0f, 0.0f};
    Body2D body_a = factory.create_body(position_a, BodyType2D::Static, nullptr);
    body_a = factory.create_box_fixture(body_a, 1.0f, 1.0f, flags);

    Vector3 position_b{5.0f, 0.0f, 0.0f};
    Body2D body_b = factory.create_body(position_b, BodyType2D::Static, nullptr);
    body_b = factory.create_box_fixture(body_b, 1.0f, 1.0f, flags);

    Body2DTransform transform_a = Body2D::get_body_transform(body_a);
    Body2DTransform transform_b = Body2D::get_body_transform(body_b);

    // act
    BodyDistance2D distance_info = world.fixture_distance(transform_a, transform_b);

    // assert
    REQUIRE(distance_info.distance == 4.0f);
    REQUIRE(distance_info.distance > 0.0f);
    REQUIRE(distance_info.normal.x != 0.0f);
    REQUIRE(distance_info.normal.y == 0.0f);
}
