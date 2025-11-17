#include <catch2/catch_test_macros.hpp>

#include <engine/physics/creation/physics_creation_factory.h>
#include <engine/physics/physics_math.h>
#include <engine/physics/raycast/physics_raycaster.h>
#include <engine/physics/raycast/collider_ray_result.h>
#include <engine/physics/world/physics_world.h>

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

    Body2D box = factory.create_body({0.0f, 5.0f, 0.0f}, BodyType2D::Static, nullptr);
    box = factory.create_box_fixture(box, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    // act
    world.step(1.0f / 60.0f);
    ColliderRayResult result = raycaster.raycast_closest(origin, translation);

    // assert
    REQUIRE(b2Shape_IsValid(result.shape_id));
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

    Body2D box1 = factory.create_body({0.0f, 3.0f, 0.0f}, BodyType2D::Static, nullptr);
    box1 = factory.create_box_fixture(box1, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    Body2D box2 = factory.create_body({0.0f, 6.0f, 0.0f}, BodyType2D::Static, nullptr);
    box2 = factory.create_box_fixture(box2, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    b2QueryFilter filter{};
    filter.categoryBits = 0xFFFF;
    filter.maskBits = 0xFFFF;

    // act
    world.step(1.0f / 60.0f);
    auto results = raycaster.raycast_all(origin, translation, filter);

    // assert
    REQUIRE(results.size() >= 2);
    REQUIRE(std::is_sorted(results.begin(), results.end(),
                           [](const ColliderRayResult& a, const ColliderRayResult& b) {
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

    Body2D box1 = factory.create_body({0.0f, 3.0f, 0.0f}, BodyType2D::Static, nullptr);
    box1 = factory.create_box_fixture(box1, {0.0f, 0.0f}, 2.0f, 2.0f, flags);
    Body2D box2 = factory.create_body({0.0f, 6.0f, 0.0f}, BodyType2D::Static, nullptr);
    box2 = factory.create_box_fixture(box2, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    world.step(1.0f / 60.0f);

    // act & assert (hits both)
    uint16_t matching_mask = 0x0002;
    auto results_matching = raycaster.raycast_filtered({0.0f, 0.0f}, {0.0f, 10.0f}, matching_mask);
    REQUIRE(results_matching.size() >= 2);

    for (const auto& r : results_matching) {
        auto filter = b2Shape_GetFilter(r.shape_id);
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

    Body2D body = factory.create_body({0.0f, 5.0f, 0.0f}, BodyType2D::Static, nullptr);
    body = factory.create_box_fixture(body, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    b2Vec2 start{0.0f, 0.0f};
    b2Vec2 end{0.0f, 10.0f};

    // act
    auto segment_result = raycaster.raycast_segment(start, end);
    auto closest_result = raycaster.raycast_closest(start, end - start);

    // assert
    REQUIRE(segment_result.shape_id.index1 == closest_result.shape_id.index1);
    REQUIRE(segment_result.shape_id.generation == closest_result.shape_id.generation);
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
    REQUIRE_FALSE(b2Shape_IsValid(result.shape_id));
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

    Body2D static_body = factory.create_body({0.0f, 3.0f, 0.0f}, BodyType2D::Static, nullptr);
    static_body = factory.create_box_fixture(static_body, {0.0f, 0.0f}, 2.0f, 2.0f, static_flags);

    Body2D dynamic_body = factory.create_body({0.0f, 6.0f, 0.0f}, BodyType2D::Dynamic, nullptr);
    dynamic_body = factory.create_box_fixture(dynamic_body, {0.0f, 0.0f}, 2.0f, 2.0f, dynamic_flags);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    world.step(1.0f / 60.0f);

    auto result = raycaster.raycast_closest(origin, translation);

    REQUIRE(b2Shape_IsValid(result.shape_id));
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
    Body2D static_body = factory.create_body({5.0f, 5.0f, 0.0f}, BodyType2D::Static, nullptr);
    static_body = factory.create_box_fixture(static_body, {0.0f, 0.0f}, 2.0f, 2.0f, static_flags);

    // dynamic box directly in the path
    Body2D dynamic_body = factory.create_body({0.0f, 5.0f, 0.0f}, BodyType2D::Dynamic, nullptr);
    dynamic_body = factory.create_box_fixture(dynamic_body, {0.0f, 0.0f}, 2.0f, 2.0f, dynamic_flags);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{0.0f, 10.0f};

    world.step(1.0f / 60.0f);

    auto result = raycaster.raycast_closest(origin, translation);

    REQUIRE(b2Shape_IsValid(result.shape_id));
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
    Body2D body1 = factory.create_body({1.0f, 2.0f, 0.0f}, BodyType2D::Dynamic, nullptr);
    body1 = factory.create_box_fixture(body1, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    Body2D body2 = factory.create_body({4.0f, 5.0f, 0.0f}, BodyType2D::Dynamic, nullptr);
    body2 = factory.create_box_fixture(body2, {0.0f, 0.0f}, 2.0f, 2.0f, flags);

    b2Vec2 origin{0.0f, 0.0f};
    b2Vec2 translation{5.0f, 5.0f};

    b2QueryFilter filter{};
    filter.categoryBits = 0xFFFF;
    filter.maskBits = 0xFFFF;

    world.step(1.0f / 60.0f);

    auto results = raycaster.raycast_all(origin, translation, filter);

    REQUIRE(results.size() >= 2);
    REQUIRE(results[0].fraction < results[1].fraction);
};
