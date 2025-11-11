#include <catch2/catch_test_macros.hpp>

#include <engine/physics/physics_creation_factory.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/physics_raycaster.h>
#include <engine/physics/physics_world.h>

#include <engine/public/util/point.h>
#include <engine/public/component.h>
#include <engine/public/gameObject.h>

TEST_CASE("physics_raycaster_closest_returns_valid_result", "[PhysicsRaycaster]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    b2BodyId box = factory.create_box_body({0.0f, 5.0f}, 2.0f, 2.0f, flags, nullptr);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    // act
    world.step();
    b2RayResult result = raycaster.raycast_closest(origin, translation);

    // assert
    REQUIRE(b2Shape_IsValid(result.shapeId));
    REQUIRE(result.fraction >= 0.0f);
    REQUIRE(result.fraction <= 1.0f);
}

TEST_CASE("physics_raycaster_all_returns_sorted_results", "[PhysicsRaycaster]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.sensor = false;
    flags.is_bullet = false;

    factory.create_box_body({0.0f, 3.0f}, 2.0f, 2.0f, flags, nullptr);
    factory.create_box_body({0.0f, 6.0f}, 2.0f, 2.0f, flags, nullptr);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    b2QueryFilter filter{};
    filter.categoryBits = 0xFFFF;
    filter.maskBits = 0xFFFF;

    // act
    world.step();
    auto results = raycaster.raycast_all(origin, translation, filter);

    // assert
    REQUIRE(results.size() >= 2);
    REQUIRE(std::is_sorted(results.begin(), results.end(),
                           [](const b2RayResult& a, const b2RayResult& b) {
                               return a.fraction < b.fraction;
                           }));
}

TEST_CASE("physics_raycaster_filtered_respects_category_mask", "[PhysicsRaycaster]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = false;
    flags.category = 0x0002; // assign category 2
    flags.mask = 0xFFFF;     // collides with all

    auto box1 = factory.create_box_body({0.0f, 3.0f}, 2.0f, 2.0f, flags, nullptr);
    auto box2 = factory.create_box_body({0.0f, 6.0f}, 2.0f, 2.0f, flags, nullptr);

    world.step();

    // act & assert (hits both)
    uint16_t matching_mask = 0x0002;
    auto results_matching = raycaster.raycast_filtered({0.0f, 0.0f}, {0.0f, 10.0f}, matching_mask);
    REQUIRE(results_matching.size() >= 2);

    for (const auto& r : results_matching) {
        auto filter = b2Shape_GetFilter(r.shapeId);
        REQUIRE((filter.categoryBits & matching_mask) != 0);
    }

    // act & assert (hits none)
    uint16_t non_matching_mask = 0x0001;
    auto results_non_matching = raycaster.raycast_filtered({0.0f, 0.0f}, {0.0f, 10.0f}, non_matching_mask);
    REQUIRE(results_non_matching.empty());
}


TEST_CASE("physics_raycaster_segment_matches_closest", "[PhysicsRaycaster]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = false;

    factory.create_box_body({0.0f, 5.0f}, 2.0f, 2.0f, flags, nullptr);

    b2Vec2 start{0.0f, 0.0f};
    b2Vec2 end{0.0f, 10.0f};

    // act
    auto segment_result = raycaster.raycast_segment(start, end);
    auto closest_result = raycaster.raycast_closest(start, end - start);

    // assert
    REQUIRE(segment_result.shapeId.index1 == closest_result.shapeId.index1);
    REQUIRE(segment_result.shapeId.generation == closest_result.shapeId.generation);
    REQUIRE(segment_result.fraction == closest_result.fraction);
}

TEST_CASE("physics_raycaster_miss_returns_invalid", "[PhysicsRaycaster]") {
    // arrange
    PhysicsWorld world;
    PhysicsRaycaster raycaster(world.world_id());

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{10.0f, 0.0f};

    // act
    auto result = raycaster.raycast_closest(origin, translation);
    
    // assert
    REQUIRE_FALSE(b2Shape_IsValid(result.shapeId));
}

TEST_CASE("physics_raycaster_closest_static_behind_dynamic", "[PhysicsRaycaster]") {
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags static_flags{};
    static_flags.dynamic = false;
    static_flags.sensor = false;

    PhysicsCreationFlags dynamic_flags{};
    dynamic_flags.dynamic = true;
    dynamic_flags.sensor = false;

    factory.create_box_body({0.0f, 3.0f}, 2.0f, 2.0f, static_flags, nullptr);

    factory.create_box_body({0.0f, 6.0f}, 2.0f, 2.0f, dynamic_flags, nullptr);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    world.step();

    auto result = raycaster.raycast_closest(origin, translation);

    REQUIRE(b2Shape_IsValid(result.shapeId));
    REQUIRE(result.fraction >= 0.0f);
    REQUIRE(result.fraction <= 0.5f);
};

TEST_CASE("physics_raycaster_misses_static_hits_dynamic", "[PhysicsRaycaster]") {
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags static_flags{};
    static_flags.dynamic = false;
    static_flags.sensor = false;

    PhysicsCreationFlags dynamic_flags{};
    dynamic_flags.dynamic = true;
    dynamic_flags.sensor = false;

    // static box off to the side
    factory.create_box_body({5.0f, 5.0f}, 2.0f, 2.0f, static_flags, nullptr);

    // dynamic box directly in the path
    factory.create_box_body({0.0f, 5.0f}, 2.0f, 2.0f, dynamic_flags, nullptr);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    world.step();

    auto result = raycaster.raycast_closest(origin, translation);

    REQUIRE(b2Shape_IsValid(result.shapeId));
    REQUIRE(result.fraction >= 0.3f);
    REQUIRE(result.fraction <= 0.6f);
};

TEST_CASE("physics_raycaster_all_multiple_bodies_at_angle", "[PhysicsRaycaster]") {
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());
    PhysicsRaycaster raycaster(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.sensor = false;

    // two boxes offset diagonally
    factory.create_box_body({1.0f, 2.0f}, 2.0f, 2.0f, flags, nullptr);
    factory.create_box_body({3.0f, 4.0f}, 2.0f, 2.0f, flags, nullptr);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{5.0f, 5.0f};

    b2QueryFilter filter{};
    filter.categoryBits = 0xFFFF;
    filter.maskBits = 0xFFFF;

    world.step();

    auto results = raycaster.raycast_all(origin, translation, filter);

    REQUIRE(results.size() >= 2);
    REQUIRE(results[0].fraction < results[1].fraction);
};
