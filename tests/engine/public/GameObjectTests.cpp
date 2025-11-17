// NOLINTFILE
#include <list>
#include <set>
#include <catch2/catch_test_macros.hpp>
#include <engine/public/gameObject.h>
#include <engine/public/scene.h>

TEST_CASE("GameObject default constructor initializes correctly", "[GameObject]") {

    // Arrange & Act
    Scene scene { " Test Scene " };
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