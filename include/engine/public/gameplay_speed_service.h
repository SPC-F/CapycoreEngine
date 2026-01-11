#pragma once

#include <engine/core/iEngineService.h>
#include <engine/input/input_system.h>
#include <engine/input/key_code.h>

/**
 * @brief Service for managing and modifying the gameplay simulation speed.
 *
 * Allows dynamic adjustment of the game's time scale, enabling features like
 * fast-forwarding, slowing down time, or resetting to default speed via input.
 *
 * Usage:
 * - Use the set keys to increase, decrease, or reset the gameplay speed.
 *   The default keys are Page Up, Page Down, and Home respectively.
 */
class GameplaySpeedService final : public IEngineService {
  /**
   * @brief Configuration settings for gameplay speed control.
   *
   * Defines limits, default values, and input bindings for controlling the
   * speed.
   */
  struct Config {
    float step = 0.1;  ///< The increment/decrement value for speed adjustments.
    float max_speed = 3;  ///< The maximum allowed gameplay speed.
    float min_speed = 0;  ///< The minimum allowed gameplay speed.
    float speed = 1;      ///< The current gameplay speed multiplier.

    float default_speed = 1;  ///< The default speed to restore upon reset.
    KeyCode increase_step_kb =
        KeyCode::page_up;  ///< Key to increase gameplay speed.
    KeyCode decrease_step_kb =
        KeyCode::page_down;  ///< Key to decrease gameplay speed.
    KeyCode reset_speed_kb = KeyCode::home;  ///< Key to reset speed to default.
  };
  Config config_;
  const IInputProvider& input_provider_;

 public:
  /**
   * @brief Constructs the GameplaySpeedService.
   *
   * @param input_provider The provider used to listen for speed control key
   * inputs.
   */
  explicit GameplaySpeedService(const IInputProvider& input_provider);
  ~GameplaySpeedService() override = default;

  /**
   * @brief Updates the service state based on user input.
   *
   * Checks for speed adjustment keys (increase, decrease, reset) and modifies
   * the current gameplay speed accordingly.
   */
  void update();

  /**
   * @brief Returns the current speed adjustment increment.
   *
   * The amount by which speed is increased or decreased per step.
   */
  [[nodiscard]] float step() const;

  /**
   * @brief Returns the maximum allowable gameplay speed.
   */
  [[nodiscard]] float max_speed() const;

  /**
   * @brief Returns the minimum allowable gameplay speed.
   */
  [[nodiscard]] float min_speed() const;

  /**
   * @brief Returns the current gameplay speed multiplier.
   *
   * A value of 1.0 represents normal speed.
   */
  [[nodiscard]] float speed() const;

  /**
   * @brief Provides mutable access to the service configuration.
   *
   * Allows runtime modification of speed limits, steps, and key bindings.
   */
  [[nodiscard]] Config& config();

  /**
   * @brief Sets the speed adjustment increment.
   *
   * @param value The new step size for speed changes.
   */
  void set_step(float value);

  /**
   * @brief Increases the gameplay speed by the specified step.
   *
   * The resulting speed is clamped to the maximum speed limit.
   * @param step The amount to add to the current speed.
   */
  void increase_speed(float step);

  /**
   * @brief Decreases the gameplay speed by the specified step.
   *
   * The resulting speed is clamped to the minimum speed limit.
   * @param step The amount to subtract from the current speed.
   */
  void decrease_speed(float step);
};