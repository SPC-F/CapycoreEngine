#include <catch2/catch_test_macros.hpp>

#include <engine/public/scene.h>
#include <engine/public/scene_service.h>
#include <string>
#include <memory>

TEST_CASE("AddScene_WhenSceneDoesNotExist_AddsSceneToSceneService", "[SceneService]") {
    // arrange & act
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    // assert
    REQUIRE(scene.name() == name);
}

TEST_CASE("AddScene_WhenSceneExists_ReturnsFoundScene", "[SceneService]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();

    // act
    Scene& dummy = scene_service.add_scene(name);
    Scene& scene = scene_service.add_scene(name);

    // assert
    REQUIRE(&dummy == &scene); // same reference, but are these value or address equal?
}

TEST_CASE("RemoveScene_WhenSceneExists_RemovesScene", "[SceneService]") {
    // arrange
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    scene_service.add_scene(name);

    // act
    scene_service.remove_scene(name);

    // assert
    REQUIRE_THROWS_AS(scene_service.load_scene(name), std::runtime_error);
}

TEST_CASE("RemoveScene_WhenSceneNotExists_Throws", "[SceneService]") {
    // arrange
    const std::string& name = "Nonexistent Scene";
    auto scene_service = SceneService();
    // act & assert
    REQUIRE_THROWS_AS(scene_service.remove_scene(name), std::runtime_error);
}

TEST_CASE("ContainedSceneNames_ReturnsAllSceneNames", "[SceneService]") {
    // arrange
    const std::string& name1 = "Scene 1";
    const std::string& name2 = "Scene 2";
    auto scene_service = SceneService();
    scene_service.add_scene(name1);
    scene_service.add_scene(name2);

    // act
    auto scene_names = scene_service.contained_scene_names();

    // assert
    REQUIRE(scene_names.size() == 2);
    REQUIRE(scene_names.find(name1) != scene_names.end());
    REQUIRE(scene_names.find(name2) != scene_names.end());
}

// Do not make these tests. They will work with the engine and kick off SDL windows. You will be hurt, do not.
//TEST_CASE("LoadScene_WhenSceneExists_SetsSceneAsRunning", "[SceneService]");
//TEST_CASE("LoadScene_WhenSceneDoesNotExists_Throws", "[SceneService]");
//TEST_CASE("AddSceneAndLoad_WhenSceneNotExist_LoadsAndAddsScene", "[SceneService]")