// NOLINTFILE
#include <list>
#include <set>
#include <catch2/catch_test_macros.hpp>
#include <engine/public/gameObject.h>
#include <engine/public/scene.h>
#include <engine/public/scene_service.h>

TEST_CASE("GameObject default constructor initializes correctly", "[GameObject]") {

    // Arrange & Act
    const std::string& name = "Test Scene";
    auto scene_service = SceneService();
    Scene& scene = scene_service.add_scene(name);

    std::list<GameObject> objects {};

    constexpr size_t object_count = 1000;

    for(size_t i = 0; i < object_count; ++i) {
        objects.emplace_back(scene);
    }

    // Assert
    std::set<std::string> ids {};
    for(const auto& obj : objects) {
        ids.insert(obj.id());
    }

    REQUIRE(ids.size() == object_count);
}