// NOLINTFILE
#include <list>
#include <set>

#include <catch2/catch_test_macros.hpp>
#include <engine/public/gameobject.h>
#include <engine/public/scene.h>

TEST_CASE("GameObject default constructor initializes correctly", "[GameObject]") {

    // Arrange & Act
    Scene scene {};
    std::list<GameObject> objects {};

    constexpr size_t object_count = 1000;

    for(int i = 0; i < object_count; ++i) {
        objects.emplace_back(scene);
    }

    // Assert
    std::set<boost::uuids::uuid> ids {};
    for(const auto& obj : objects) {
        ids.insert(obj.id());
    }

    REQUIRE(ids.size() == object_count);
}