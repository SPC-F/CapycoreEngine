#include <engine/core/engine.h>
#include <engine/public/gameplay_speed_service.h>

GameplaySpeedService::GameplaySpeedService(const IInputProvider& input_provider)
    : input_provider_(input_provider) {}

void GameplaySpeedService::update() {
  if (input_provider_.is_key_pressed(config_.increase_step_kb)) {
    increase_speed(config_.step);
  }

  if (input_provider_.is_key_pressed(config_.decrease_step_kb)) {
    decrease_speed(config_.step);
  }

  if (input_provider_.is_key_pressed(config_.reset_speed_kb)) {
    config_.speed = config_.default_speed;
  }
}

float GameplaySpeedService::step() const { return config_.step; }

float GameplaySpeedService::max_speed() const { return config_.max_speed; }

float GameplaySpeedService::min_speed() const { return config_.min_speed; }

float GameplaySpeedService::speed() const { return config_.speed; }

GameplaySpeedService::Config& GameplaySpeedService::config() { return config_; }

void GameplaySpeedService::set_step(const float value) {
  if (value > 0) {
    config_.step = value;
  }
}

void GameplaySpeedService::increase_speed(const float step) {
  config_.speed = std::min(config_.speed + step, config_.max_speed);
}

void GameplaySpeedService::decrease_speed(const float step) {
  config_.speed = std::max(config_.speed - step, config_.min_speed);
}
