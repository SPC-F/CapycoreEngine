#include <catch2/catch_test_macros.hpp>

#include <engine/public/scene.h>
#include <engine/public/gameObject.h>
#include <engine/public/scene_service.h>
#include <string>
#include <memory>

TEST_CASE("CreateScene_CreatesValidScene", "[Scene]") {
    // arrange & act
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    // assert
    REQUIRE(scene.name() == name);
    REQUIRE(scene.game_objects().empty());
    REQUIRE(!scene.is_running());
    REQUIRE(scene.time_scale() == 1.0f);
}

TEST_CASE("AddGameObject_AddsGameObjectToScene_WhenExistingGameObject", "[Scene]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    const std::string& game_object_name = "Test GameObject";

    // act
    auto& game_object = scene.add_game_object(game_object_name);

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(game_object.id()).get().id() == game_object.id());
}

TEST_CASE("AddGameObject_AddsGameObjectToScene_FromName", "[Scene]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    const std::string& game_object_name = "Test GameObject";

    // act
    auto& game_object = scene.add_game_object(game_object_name);

    auto& retrieved_game_object = scene.get_game_object(game_object.id()).get();
    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(retrieved_game_object.id() == game_object.id());
}

TEST_CASE("AddGameObjects_AddsMultipleGameObjectsToScene", "[Scene]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    auto game_object_1 = std::make_unique<GameObject>(scene);
    auto game_object_2 = std::make_unique<GameObject>(scene);
    const auto game_object_1_id = game_object_1->id();
    const auto game_object_2_id = game_object_2->id();
    std::vector<std::unique_ptr<GameObject>> game_objects;
    game_objects.emplace_back(std::move(game_object_1));
    game_objects.emplace_back(std::move(game_object_2));

    // act
    scene.add_game_objects(std::move(game_objects));

    // assert
    REQUIRE(scene.game_objects().size() == 2);
    REQUIRE(scene.get_game_object(game_object_1_id).get().id() == game_object_1_id);
    REQUIRE(scene.get_game_object(game_object_2_id).get().id() == game_object_2_id);
}

TEST_CASE("RemoveGameObject_RemovesRightGameObjectFromScene", "[Scene]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    const std::string& game_object_name_1 = "Test GameObject 1";
    const std::string& game_object_name_2 = "Test GameObject 2";

    auto& obj1 = scene.add_game_object(game_object_name_1);
    auto& obj2 = scene.add_game_object(game_object_name_2);

    REQUIRE(scene.game_objects().size() == 2);

    // act
    scene.remove_game_object(obj1);

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(obj2.id()).get().id() == obj2.id());
}

TEST_CASE("RemoveGameObject_RemovesGameObjectWithAllChildObjects", "[Scene]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    const std::string& parent_name = "Parent GameObject";
    const std::string& child_name = "Child GameObject";

    auto& parent = scene.add_game_object(parent_name);
    auto& child = scene.add_game_object(child_name);
    child.parent(parent);

    // act
    scene.remove_game_object(parent);

    // assert
    REQUIRE(scene.game_objects().empty());
}

// Do not make these tests. They will call the engine singleton and give you a big headache.
//TEST_CASE("OnRunListeners_AreCalledWhenSceneRuns", "[Scene]")
//TEST_CASE("OnStopListeners_AreCalledWhenSceneStops", "[Scene]")
//TEST_CASE("OnDestroyListeners_AreCalledWhenSceneSceneDestroyed", "[Scene]")