#include <catch2/catch_test_macros.hpp>

#include <engine/input/key_state.h>

TEST_CASE("key_state_reset_should_set_current_and_previous_to_false", "[KeyState]")
{
    KeyState key_state;

    key_state.current = true;
    key_state.previous = true;

    key_state.reset();

    REQUIRE(key_state.current == false);
    REQUIRE(key_state.previous == false);
}
