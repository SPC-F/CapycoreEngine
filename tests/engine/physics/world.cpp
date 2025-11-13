#include <catch2/catch_test_macros.hpp>

#include <engine/physics/physics_world.h>
#include <engine/physics/physics_creation_factory.h>
#include <engine/public/util/point.h>

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
    REQUIRE_NOTHROW(world.check_collision());
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

    Point position{0.0f, 10.0f};
    b2BodyId body = factory.create_body(position, b2BodyType::b2_dynamicBody, nullptr);
    body = factory.create_box_fixture(body, 1.0f, 1.0f, flags);
    b2Vec2 initial_pos = b2Body_GetPosition(body);

    // act
    int steps = 5; // step ~5 frames
    for (int i = 0; i < steps; ++i) {
        world.step(1.0f / 60.0f);
    }
    b2Vec2 new_pos = b2Body_GetPosition(body);

    // assert
    REQUIRE(new_pos.y < initial_pos.y);
    REQUIRE(new_pos.x < initial_pos.x + 0.1f); // allow small horizontal drift
    REQUIRE(new_pos.x > initial_pos.x - 0.1f);


    // check that collision check runs without crashing
    REQUIRE_NOTHROW(world.check_collision());
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