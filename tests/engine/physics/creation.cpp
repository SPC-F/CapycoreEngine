#include <catch2/catch_test_macros.hpp>

#include <engine/physics/physics_creation_factory.h>
#include <engine/physics/physics_world.h>

#include <engine/public/component.h>
#include <engine/public/gameObject.h>
#include <engine/public/util/point.h>
#include <engine/public/scene.h>

struct DummyScene : public Scene {
};

struct DummyGameObject : public GameObject {
    explicit DummyGameObject(Scene& scene) : GameObject(scene) {}
};

struct DummyComponent : public Component {
    explicit DummyComponent(GameObject& parent) : Component(parent) {}
    void update() override {}
    void on_attach() override {}
    void on_detach() override {}
    void on_serialize() override {}
    void on_deserialize() override {}
};

TEST_CASE("physics_creation_factory_creates_body", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    Point position{0.0f, 0.0f};

    DummyScene dummy_scene;
    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_body(position, b2BodyType::b2_staticBody, &dummy_component);

    // assert
    REQUIRE(b2Body_IsValid(body_id));
    b2Vec2 pos = b2Body_GetPosition(body_id);
    REQUIRE(pos.x == position.x);
    REQUIRE(pos.y == position.y);
    REQUIRE(b2Body_GetType(body_id) == b2BodyType::b2_staticBody);
}


TEST_CASE("physics_creation_factory_creates_box_fixture", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.is_bullet = false;
    flags.sensor = false;

    Point position{5.0f, 10.0f};

    DummyScene dummy_scene;
    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_body(position, b2BodyType::b2_dynamicBody, &dummy_component);
    body_id = factory.create_box_fixture(body_id, 2.0f, 4.0f, flags);
    
    // assert
    REQUIRE(b2Body_IsValid(body_id));
    b2Vec2 pos = b2Body_GetPosition(body_id);
    REQUIRE(pos.x == position.x);
    REQUIRE(pos.y == position.y);
    REQUIRE(b2Body_GetType(body_id) == b2BodyType::b2_dynamicBody);
}

TEST_CASE("physics_creation_factory_creates_circle_fixture", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = false;
    flags.enable_contact_events = false;
    flags.sensor = true;

    Point position{0.0f, 0.0f};

    DummyScene dummy_scene;
    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component(dummy_game_object);


    // act
    b2BodyId body_id = factory.create_body(position, b2BodyType::b2_staticBody, &dummy_component);
    body_id = factory.create_circle_fixture(body_id, 1.0f, flags);

    // assert
    REQUIRE(b2Body_IsValid(body_id));
    REQUIRE(b2Body_GetType(body_id) == b2BodyType::b2_staticBody);
}

TEST_CASE("physics_creation_factory_creates_bullet_body_with_mass_data", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.is_bullet = true;

    Point position{2.0f, 3.0f};

    DummyScene dummy_scene;
    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_body(position, b2BodyType::b2_dynamicBody, &dummy_component);
    body_id = factory.create_circle_fixture(body_id, 0.5f, flags);

    // assert
    REQUIRE(b2Body_IsValid(body_id));
    REQUIRE(b2Body_IsBullet(body_id));

    b2MassData mass_data = b2Body_GetMassData(body_id);
    REQUIRE(mass_data.mass > 0.0f);
}

TEST_CASE("physics_creation_factory_destroys_body", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;

    Point position{1.0f, 1.0f};

    b2BodyId body_id = factory.create_body(position, b2BodyType::b2_dynamicBody, nullptr);
    body_id = factory.create_box_fixture(body_id, 2.0f, 2.0f, flags);
    REQUIRE(b2Body_IsValid(body_id));

    // act
    factory.destroy_body(body_id);

    // assert
    REQUIRE_FALSE(b2Body_IsValid(body_id));
}
