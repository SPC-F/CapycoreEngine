#include <iostream>
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
    // act and assert
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

TEST_CASE("LoadScene_MovesDontDestroyOnLoadObjects_ToNewScene", "[SceneService]")
{
    // arrange
    auto scene_service = SceneService();
    Scene& scene1 = scene_service.add_scene("Scene1");
    Scene& scene2 = scene_service.add_scene("Scene2");

    GameObject& obj = scene1.add_game_object("PersistentObject");
    obj.mark_dont_destroy_on_load(true);
    std::string obj_id = obj.id();

    // Set up Scene 2 to stop immediately when run, returning control to the caller
    scene2.on_run([](Scene& s)
    {
        s.stop();
    });

    // Trigger the load of Scene 2 from within Scene 1
    // This ensures Scene 1 is marked as 'running' when the transition logic executes
    scene1.on_run([&](Scene& s)
    {
        scene_service.load_scene("Scene2");
    });

    // act
    scene_service.load_scene("Scene1");

    // assert
    REQUIRE(scene2.game_objects().size() == 1);
    REQUIRE(scene2.game_objects()[0].get().id() == obj_id);
    REQUIRE(scene2.game_objects()[0].get().scene().name() == "Scene2");
}

TEST_CASE("LoadScene_DoesNotMoveNormalObjects_ToNewScene", "[SceneService]")
{
    // arrange
    auto scene_service = SceneService();
    Scene& scene1 = scene_service.add_scene("Scene1");
    Scene& scene2 = scene_service.add_scene("Scene2");

    scene1.add_game_object("NormalObject");
    // Not marking the object as dont_destroy_on_load

    scene2.on_run([](Scene& s)
    {
        s.stop();
    });

    scene1.on_run([&](Scene& s)
    {
        scene_service.load_scene("Scene2");
    });

    // act
    scene_service.load_scene("Scene1");

    // assert
    REQUIRE(scene2.game_objects().empty());
}

TEST_CASE("LoadScene_MovesDontDestroyOnLoadObject_WithChildren", "[SceneService]")
{
    // arrange
    auto scene_service = SceneService();
    Scene& scene1 = scene_service.add_scene("Scene1");
    Scene& scene2 = scene_service.add_scene("Scene2");

    GameObject& parent = scene1.add_game_object("Parent");
    parent.mark_dont_destroy_on_load(true);
    std::string parent_id = parent.id();

    GameObject& child = scene1.add_game_object("Child");
    parent.add_child(child);
    std::string child_id = child.id();

    scene2.on_run([](Scene& s)
    {
        s.stop();
    });

    scene1.on_run([&](Scene& s)
    {
        scene_service.load_scene("Scene2");
    });

    // act
    scene_service.load_scene("Scene1");

    // assert
    auto scene2_objects = scene2.game_objects();
    auto found_parent_it = std::ranges::find_if(scene2_objects,
                                                [&](const auto& obj) { return obj.get().id() == parent_id; });
    REQUIRE(found_parent_it != scene2_objects.end());

    GameObject& moved_parent = found_parent_it->get();
    REQUIRE(moved_parent.scene().name() == "Scene2");

    // Child should be a child of a parent and have the correct scene
    REQUIRE(moved_parent.children().size() == 1);
    GameObject& moved_child = moved_parent.children()[0].get();
    REQUIRE(moved_child.id() == child_id);
    REQUIRE(moved_child.scene().name() == "Scene2");
}

TEST_CASE("LoadScene_DoesNotMoveChildMarkedDontDestroy_IfParentIsNotMarked", "[SceneService]")
{
    // arrange
    auto scene_service = SceneService();
    Scene& scene1 = scene_service.add_scene("Scene1");
    Scene& scene2 = scene_service.add_scene("Scene2");

    GameObject& parent = scene1.add_game_object("Parent");
    // Don't mark the parent object

    GameObject& child = scene1.add_game_object("Child");
    child.mark_dont_destroy_on_load(true);
    parent.add_child(child);

    scene2.on_run([](Scene& s)
    {
        s.stop();
    });

    scene1.on_run([&](Scene& s)
    {
        scene_service.load_scene("Scene2");
    });

    // act
    scene_service.load_scene("Scene1");

    // assert
    REQUIRE(scene2.game_objects().empty());
}

// Do not make these tests. They will work with the engine and kick off SDL windows. You will be hurt, do not.
//TEST_CASE("LoadScene_WhenSceneExists_SetsSceneAsRunning", "[SceneService]");
//TEST_CASE("LoadScene_WhenSceneDoesNotExists_Throws", "[SceneService]");
//TEST_CASE("AddSceneAndLoad_WhenSceneNotExist_LoadsAndAddsScene", "[SceneService]")