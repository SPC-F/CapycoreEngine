#include <catch2/catch_test_macros.hpp>

#include <engine/public/scene.h>
#include <engine/public/gameObject.h>
#include <string>
#include <memory>

TEST_CASE("CreateScene_CreatesValidScene", "[Scene]") {
    // arrange & act
    const auto& name = "Test Scene";
    Scene scene(name);

    // assert
    REQUIRE(scene.name() == name);
    REQUIRE(scene.game_objects().empty());
    REQUIRE(!scene.is_running());
    REQUIRE(scene.time_scale() == 1.0f);
}

TEST_CASE("AddGameObject_AddsGameObjectToScene_WhenExistingGameObject", "[Scene]") {
    // arrange
    Scene scene("Test Scene");
    const char * const game_object_name = "Test GameObject";
    auto game_object = std::make_unique<GameObject>(scene);
    game_object->name(game_object_name);

    // act
    scene.add_game_object(std::move(game_object));

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(game_object_name).get().name() == game_object_name);
}

TEST_CASE("AddGameObject_AddsGameObjectToScene_FromName", "[Scene]") {
    // arrange
    Scene scene("Test Scene");
    const char * const game_object_name = "Test GameObject";

    // act
    scene.add_game_object(game_object_name);

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(game_object_name).get().name() == game_object_name);
}

TEST_CASE("AddGameObjects_AddsMultipleGameObjectsToScene", "[Scene]") {
    // arrange
    Scene scene("Test Scene");
    const char * const game_object_name_1 = "Test GameObject 1";
    const char * const game_object_name_2 = "Test GameObject 2";

    auto game_object_1 = std::make_unique<GameObject>(scene);
    game_object_1->name(game_object_name_1);

    auto game_object_2 = std::make_unique<GameObject>(scene);
    game_object_2->name(game_object_name_2);

    std::vector<std::unique_ptr<GameObject>> game_objects;
    game_objects.emplace_back(std::move(game_object_1));
    game_objects.emplace_back(std::move(game_object_2));

    // act
    scene.add_game_objects(std::move(game_objects));

    // assert
    REQUIRE(scene.game_objects().size() == 2);
    REQUIRE(scene.get_game_object(game_object_name_1).get().name() == game_object_name_1);
    REQUIRE(scene.get_game_object(game_object_name_2).get().name() == game_object_name_2);
}

TEST_CASE("RemoveGameObject_RemovesRightGameObjectFromScene", "[Scene]") {
    // arrange
    Scene scene("Test Scene");
    const char * const game_object_name_1 = "Test GameObject 1";
    const char * const game_object_name_2 = "Test GameObject 2";

    scene.add_game_object(game_object_name_1);
    scene.add_game_object(game_object_name_2);

    REQUIRE(scene.game_objects().size() == 2);

    // act
    scene.remove_game_object(game_object_name_1);

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(game_object_name_2).get().name() == game_object_name_2);
}

TEST_CASE("RemoveGameObject_RemovesAllWithSameName", "[Scene]") {
    // arrange
    Scene scene("Test Scene");
    const char * const game_object_name = "Test GameObject";

    scene.add_game_object(game_object_name);
    scene.add_game_object(game_object_name);

    REQUIRE(scene.game_objects().size() == 2);

    // act
    scene.remove_game_object(game_object_name);

    // assert
    REQUIRE(scene.game_objects().size() == 1);
    REQUIRE(scene.get_game_object(game_object_name).get().name() == game_object_name);
}