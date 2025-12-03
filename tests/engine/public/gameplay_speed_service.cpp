#include <engine/input/input_system.h>
#include <engine/public/gameplay_speed_service.h>

#include <catch2/catch_test_macros.hpp>

class TestableInputSystem final : public InputSystem {
 public:
  using InputSystem::key_states_;
};

TEST_CASE("step_should_return_configured_step", "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().step = 0.5f;

  REQUIRE(service.step() == 0.5f);
}

TEST_CASE("max_speed_should_return_configured_max_speed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().max_speed = 5.0f;

  REQUIRE(service.max_speed() == 5.0f);
}

TEST_CASE("min_speed_should_return_configured_min_speed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().min_speed = 0.1f;

  REQUIRE(service.min_speed() == 0.1f);
}

TEST_CASE("speed_should_return_default_speed_initially",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().default_speed = 1.0f;
  service.config().speed = 1.0f;

  REQUIRE(service.speed() == 1.0f);
}

TEST_CASE("set_step_should_update_step_when_value_is_valid",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().min_speed = 0.0f;
  service.set_step(0.2f);

  REQUIRE(service.step() == 0.2f);
}

TEST_CASE("set_step_should_not_update_step_when_value_is_invalid",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().min_speed = 1.0f;
  service.config().step = 0.5f;

  service.set_step(0.5f);  // 0.5 is not > 1.0

  REQUIRE(service.step() == 0.5f);
}

TEST_CASE("increase_speed_should_increase_speed_by_step",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 1.0f;
  service.config().max_speed = 2.0f;

  service.increase_speed(0.1f);

  REQUIRE(service.speed() == 1.1f);
}

TEST_CASE("increase_speed_should_clamp_to_max_speed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 1.9f;
  service.config().max_speed = 2.0f;

  service.increase_speed(0.2f);

  REQUIRE(service.speed() == 2.0f);
}

TEST_CASE("decrease_speed_should_decrease_speed_by_step",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 1.0f;
  service.config().min_speed = 0.0f;

  service.decrease_speed(0.1f);

  REQUIRE(service.speed() == 0.9f);
}

TEST_CASE("decrease_speed_should_clamp_to_min_speed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 0.1f;
  service.config().min_speed = 0.0f;

  service.decrease_speed(0.2f);

  REQUIRE(service.speed() == 0.0f);
}

TEST_CASE("update_should_increase_speed_when_increase_key_pressed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 1.0f;
  service.config().step = 0.1f;
  service.config().increase_step_kb = KeyCode::page_up;

  // Simulate key press (transition from released to pressed)
  input.key_states_[KeyCode::page_up].previous = false;
  input.key_states_[KeyCode::page_up].current = true;

  service.update();

  REQUIRE(service.speed() == 1.1f);
}

TEST_CASE("update_should_decrease_speed_when_decrease_key_pressed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 1.0f;
  service.config().step = 0.1f;
  service.config().decrease_step_kb = KeyCode::page_down;

  // Simulate key press
  input.key_states_[KeyCode::page_down].previous = false;
  input.key_states_[KeyCode::page_down].current = true;

  service.update();

  REQUIRE(service.speed() == 0.9f);
}

TEST_CASE("update_should_reset_speed_when_reset_key_pressed",
          "[GameplaySpeedService]") {
  TestableInputSystem input;
  GameplaySpeedService service(input);

  service.config().speed = 2.0f;
  service.config().default_speed = 1.0f;
  service.config().reset_speed_kb = KeyCode::home;

  // Simulate key press
  input.key_states_[KeyCode::home].previous = false;
  input.key_states_[KeyCode::home].current = true;

  service.update();

  REQUIRE(service.speed() == 1.0f);
}