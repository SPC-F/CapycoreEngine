#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

#include <engine/storage/simple_storage.h>

class TestableStorage : public SimpleStorage {
public:
    using SimpleStorage::session_storage_;

    static TestableStorage & instance()
    {
        static TestableStorage inst;
        return inst;
    }

    void test_load() { load(); }
    void test_save() { save(); }
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

TEST_CASE("save_writes_file_and_load_restores_int_float_string", "[SimpleStorage][Persistence]")
{
    auto& storage = TestableStorage::instance();
    storage.delete_all();

    storage.set_value("i", 123);
    storage.set_value("f", 4.5f);
    storage.set_value("s", std::string("hello world"));

    storage.save();

    storage.delete_all();
    storage.test_load();

    REQUIRE(storage.try_get("i").has_value());
    REQUIRE(std::get<int>(*storage.try_get("i")) == 123);

    REQUIRE(storage.try_get("f").has_value());
    REQUIRE(std::get<float>(*storage.try_get("f")) == Catch::Approx(4.5f));

    REQUIRE(storage.try_get("s").has_value());
    REQUIRE(std::get<std::string>(*storage.try_get("s")) == "hello world");
}

TEST_CASE("save_then_load_preserves_overwritten_values", "[SimpleStorage][Persistence]")
{
    auto& storage = TestableStorage::instance();
    storage.delete_all();

    storage.set_value("a", 1);
    storage.set_value("a", std::string("updated"));
    storage.set_value("b", 3.14f);

    storage.save();

    storage.delete_all();
    storage.test_load();

    REQUIRE(storage.try_get("a").has_value());
    REQUIRE(std::get<std::string>(*storage.try_get("a")) == "updated");

    REQUIRE(storage.try_get("b").has_value());
    REQUIRE(std::get<float>(*storage.try_get("b")) == Catch::Approx(3.14f));
}

TEST_CASE("load_ignores_malformed_lines", "[SimpleStorage][Persistence]")
{
    auto& storage = TestableStorage::instance();
    storage.delete_all();

    {
        std::filesystem::create_directories("saved");
        std::ofstream out("saved/simple.dat", std::ios::trunc);
        out << "good=42\n";
        out << "bad_line_without_equals\n"; // Malformed line
        out << "also_bad=\n";
        out << "str=\"abc\"\n";
        out << "float=1.25\n";
    }

    storage.test_load();

    REQUIRE(storage.try_get("good").has_value());
    REQUIRE(std::get<int>(*storage.try_get("good")) == 42);

    REQUIRE(storage.try_get("str").has_value());
    REQUIRE(std::get<std::string>(*storage.try_get("str")) == "abc");

    REQUIRE(storage.try_get("float").has_value());
    REQUIRE(std::get<float>(*storage.try_get("float")) == Catch::Approx(1.25f));

    REQUIRE(storage.try_get("bad_line_without_equals").has_value() == false);
    REQUIRE(storage.try_get("also_bad").has_value() == false);
}

TEST_CASE("load_handles_escaped_strings", "[SimpleStorage][Persistence]")
{
    auto& storage = TestableStorage::instance();
    storage.delete_all();

    {
        std::filesystem::create_directories("saved");
        std::ofstream out("saved/simple.dat", std::ios::trunc);
        out << "escaped=\"hello \\\"world\\\"\"\n";
    }

    storage.test_load();

    REQUIRE(storage.try_get("escaped").has_value());
    REQUIRE(std::get<std::string>(*storage.try_get("escaped")) == "hello \"world\"");
}

TEST_CASE("save_overwrites_previous_file", "[SimpleStorage][Persistence]")
{
    auto& storage = TestableStorage::instance();
    storage.delete_all();

    storage.set_value("old", 1);
    storage.save();

    storage.delete_all();
    storage.set_value("new", 999);
    storage.save();

    storage.delete_all();
    storage.test_load();

    REQUIRE(storage.try_get("old").has_value() == false);
    REQUIRE(storage.try_get("new").has_value());
    REQUIRE(std::get<int>(*storage.try_get("new")) == 999);
}

