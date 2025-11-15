#include <catch2/catch_test_macros.hpp>

#include <engine/public/util/vector3.h>

TEST_CASE("default_constructor_initializes_to_zero", "[Vector3]")
{
    Vector3 v;
    REQUIRE(v.x == 0.0f);
    REQUIRE(v.y == 0.0f);
    REQUIRE(v.z == 0.0f);
}

TEST_CASE("parameterized_constructor_initializes_correctly", "[Vector3]")
{
    Vector3 v(1.0f, 2.0f, 3.0f);
    REQUIRE(v.x == 1.0f);
    REQUIRE(v.y == 2.0f);
    REQUIRE(v.z == 3.0f);
}

TEST_CASE("vector_addition_returns_correct_result", "[Vector3]")
{
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);

    Vector3 r = a + b;

    REQUIRE(r.x == 5.0f);
    REQUIRE(r.y == 7.0f);
    REQUIRE(r.z == 9.0f);
}

TEST_CASE("vector_subtraction_returns_correct_result", "[Vector3]")
{
    Vector3 a(5, 7, 9);
    Vector3 b(1, 2, 3);

    Vector3 r = a - b;

    REQUIRE(r.x == 4.0f);
    REQUIRE(r.y == 5.0f);
    REQUIRE(r.z == 6.0f);
}

TEST_CASE("vector_multiplication_elementwise_returns_correct_result", "[Vector3]")
{
    Vector3 a(2, 3, 4);
    Vector3 b(5, 6, 7);

    Vector3 r = a * b;

    REQUIRE(r.x == 10.0f);
    REQUIRE(r.y == 18.0f);
    REQUIRE(r.z == 28.0f);
}

TEST_CASE("vector_division_elementwise_returns_correct_result", "[Vector3]")
{
    Vector3 a(10, 18, 28);
    Vector3 b(2, 3, 4);

    Vector3 r = a / b;

    REQUIRE(r.x == 5.0f);
    REQUIRE(r.y == 6.0f);
    REQUIRE(r.z == 7.0f);
}

TEST_CASE("compound_addition_mutates_correctly", "[Vector3]")
{
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);

    a += b;

    REQUIRE(a.x == 5.0f);
    REQUIRE(a.y == 7.0f);
    REQUIRE(a.z == 9.0f);
}

TEST_CASE("compound_subtraction_mutates_correctly", "[Vector3]")
{
    Vector3 a(5, 7, 9);
    Vector3 b(1, 2, 3);

    a -= b;

    REQUIRE(a.x == 4.0f);
    REQUIRE(a.y == 5.0f);
    REQUIRE(a.z == 6.0f);
}

TEST_CASE("compound_multiplication_mutates_correctly", "[Vector3]")
{
    Vector3 a(2, 3, 4);
    Vector3 b(5, 6, 7);

    a *= b;

    REQUIRE(a.x == 10.0f);
    REQUIRE(a.y == 18.0f);
    REQUIRE(a.z == 28.0f);
}

TEST_CASE("compound_division_mutates_correctly", "[Vector3]")
{
    Vector3 a(10, 18, 28);
    Vector3 b(2, 3, 4);

    a /= b;

    REQUIRE(a.x == 5.0f);
    REQUIRE(a.y == 6.0f);
    REQUIRE(a.z == 7.0f);
}

TEST_CASE("scalar_multiplication_returns_correct_result", "[Vector3]")
{
    Vector3 a(2, 3, 4);

    Vector3 r = a * 2.0f;

    REQUIRE(r.x == 4.0f);
    REQUIRE(r.y == 6.0f);
    REQUIRE(r.z == 8.0f);
}

TEST_CASE("scalar_division_returns_correct_result", "[Vector3]")
{
    Vector3 a(4, 6, 8);

    Vector3 r = a / 2.0f;

    REQUIRE(r.x == 2.0f);
    REQUIRE(r.y == 3.0f);
    REQUIRE(r.z == 4.0f);
}

TEST_CASE("compound_scalar_multiplication_mutates_correctly", "[Vector3]")
{
    Vector3 a(2, 3, 4);

    a *= 3.0f;

    REQUIRE(a.x == 6.0f);
    REQUIRE(a.y == 9.0f);
    REQUIRE(a.z == 12.0f);
}

TEST_CASE("compound_scalar_division_mutates_correctly", "[Vector3]")
{
    Vector3 a(6, 9, 12);

    a /= 3.0f;

    REQUIRE(a.x == 2.0f);
    REQUIRE(a.y == 3.0f);
    REQUIRE(a.z == 4.0f);
}

TEST_CASE("chained_operations_produce_correct_results", "[Vector3]")
{
    Vector3 a(1, 2, 3);
    Vector3 b(1, 1, 1);

    Vector3 r = (a + b) * 2.0f - Vector3(1, 2, 3);

    REQUIRE(r.x == 3.0f);
    REQUIRE(r.y == 4.0f);
    REQUIRE(r.z == 5.0f);
}

TEST_CASE("self_assignment_is_safe", "[Vector3]")
{
    Vector3 a(2, 3, 4);

    a += a;
    REQUIRE(a.x == 4.0f);
    REQUIRE(a.y == 6.0f);
    REQUIRE(a.z == 8.0f);

    a *= a;
    REQUIRE(a.x == 16.0f);
    REQUIRE(a.y == 36.0f);
    REQUIRE(a.z == 64.0f);
}
