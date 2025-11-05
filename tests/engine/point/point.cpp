#include <catch2/catch_test_macros.hpp>

#include <engine/public/util/point.h>

TEST_CASE("Point default constructor initializes to (0,0)", "[Point]") {
    Point p;
    REQUIRE(p.x == 0.0f);
    REQUIRE(p.y == 0.0f);
}
TEST_CASE("Point parameterized constructor initializes correctly", "[Point]") {
    Point p(3.5f, -2.5f);
    REQUIRE(p.x == 3.5f);
    REQUIRE(p.y == -2.5f);
}