#include <catch2/catch_test_macros.hpp>

#include <engine/physics/physics_creation_factory.h>
#include <engine/physics/physics_world.h>

#include <engine/public/component.h>
#include <engine/public/gameObject.h>
#include <engine/public/util/point.h>
#include <engine/public/scene.h>
#include <engine/public/scene_service.h>

struct DummyGameObject : public GameObject {
    explicit DummyGameObject(Scene& scene) : GameObject(scene) {}
};

struct DummyComponent : public Component {
    explicit DummyComponent() : Component() {}
    void update(float dt) override {}
    void on_attach() override {}
    void on_detach() override {}
    void on_serialize() override {}
    void on_deserialize() override {}
};


TEST_CASE("physics_creation_factory_creates_box_body", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = true;
    flags.enable_contact_events = false;
    flags.is_bullet = false;
    flags.sensor = false;

    Point position{5.0f, 10.0f};

    const std::string& scene_name = "Test Scene";
    auto scene_service = SceneService();
    Scene& dummy_scene = scene_service.add_scene(scene_name);

    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component;
    dummy_component.parent(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_box_body(position, 2.0f, 4.0f, flags, &dummy_component);

    // assert
    REQUIRE(b2Body_IsValid(body_id));
    b2Vec2 pos = b2Body_GetPosition(body_id);
    REQUIRE(pos.x == position.x);
    REQUIRE(pos.y == position.y);
    REQUIRE(b2Body_GetType(body_id) == b2BodyType::b2_dynamicBody);
}

TEST_CASE("physics_creation_factory_creates_circle_body", "[PhysicsCreationFactory]") {
    // arrange
    PhysicsWorld world;
    PhysicsCreationFactory factory(world.world_id());

    PhysicsCreationFlags flags{};
    flags.dynamic = false;
    flags.enable_contact_events = false;
    flags.sensor = true;

    Point position{0.0f, 0.0f};

    const std::string& scene_name = "Test Scene";
    auto scene_service = SceneService();
    Scene& dummy_scene = scene_service.add_scene(scene_name);

    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component;
    dummy_component.parent(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_circle_body(position, 1.0f, flags, &dummy_component);

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

    const std::string& scene_name = "Test Scene";
    auto scene_service = SceneService();
    Scene& dummy_scene = scene_service.add_scene(scene_name);

    DummyGameObject dummy_game_object(dummy_scene);
    DummyComponent dummy_component;
    dummy_component.parent(dummy_game_object);

    // act
    b2BodyId body_id = factory.create_circle_body(position, 0.5f, flags, &dummy_component);

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
    b2BodyId body_id = factory.create_box_body(position, 2.0f, 2.0f, flags, nullptr);
    REQUIRE(b2Body_IsValid(body_id));

    // act
    factory.destroy_body(body_id);

    // assert
    REQUIRE_FALSE(b2Body_IsValid(body_id));
}
