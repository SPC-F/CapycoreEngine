#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <engine/storage/simple_storage.h>

class TestableStorage : public SimpleStorage {
public:
    using SimpleStorage::session_storage_;
};

TEST_CASE("try_get_returns_empty_optional_when_key_missing", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    auto result = storage.try_get("missing_key");
    REQUIRE(result.has_value() == false);
}

TEST_CASE("try_get_returns_value_when_key_exists", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    storage.set_value("answer", 42);

    auto result = storage.try_get("answer");
    REQUIRE(result.has_value() == true);
    REQUIRE(std::get<int>(*result) == 42);
}

TEST_CASE("set_value_overwrites_existing_value", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    storage.set_value("score", 10);
    storage.set_value("score", 3.14f);

    auto result = storage.try_get("score");
    REQUIRE(result.has_value() == true);
    REQUIRE(std::get<float>(*result) == Catch::Approx(3.14f));
}

TEST_CASE("delete_key_removes_only_that_key", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    storage.set_value("a", 1);
    storage.set_value("b", 2);

    storage.delete_key("a");

    REQUIRE(storage.try_get("a").has_value() == false);
    REQUIRE(storage.try_get("b").has_value() == true);
}

TEST_CASE("delete_key_is_safe_when_key_missing", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    storage.set_value("present", 123);
    storage.delete_key("ghost");

    REQUIRE(storage.try_get("present").has_value() == true);
}

TEST_CASE("delete_all_removes_every_entry", "[SimpleStorage]")
{
    auto& storage = SimpleStorage::instance();
    storage.delete_all();

    storage.set_value("x", 99);
    storage.set_value("y", "hello");

    storage.delete_all();

    REQUIRE(storage.try_get("x").has_value() == false);
    REQUIRE(storage.try_get("y").has_value() == false);
}
