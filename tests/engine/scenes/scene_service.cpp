#include <catch2/catch_test_macros.hpp>

#include <engine/public/scene.h>
#include <engine/public/scene_service.h>
#include <string>
#include <memory>

TEST_CASE("SceneService_LoadsAndRetrievesCurrentScene", "[SceneService]") {
    // arrange
    SceneService scene_service;
    const std::string& scene_name = "Loaded Scene";
    auto new_scene = std::make_unique<Scene>(scene_name);

    // act
    scene_service.load_scene(std::move(new_scene));
    Scene& current_scene = scene_service.current_scene();

    // assert
    REQUIRE(current_scene.name() == scene_name);
}

TEST_CASE("SceneService_DefaultSceneOnInitialization", "[SceneService]") {
    // arrange
    const std::string& default_scene_name = "default";

    // act
    SceneService scene_service {};
    Scene& current_scene = scene_service.current_scene();

    // assert
    REQUIRE(current_scene.name() == default_scene_name);
}