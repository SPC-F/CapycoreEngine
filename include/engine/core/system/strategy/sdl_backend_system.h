#pragma once

#include <SDL3/SDL.h>
#include <engine/core/system/i_backend_system.h>

/**
 * @brief SDL-based implementation of the backend system interface.
 *
 * Handles event polling and frame timing using SDL functions.
 */
class SDLBackendSystem : public IBackendSystem {
 public:
  SDLBackendSystem() = default;

  void initialize_listeners(
      std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners)
      override;

  /**
   * @brief Polls for new events from the backend system.
   *
   * Should invoke registered callbacks for each event type as events are
   * received and reinterpreted from SDL events.
   */
  void poll_events() override;

  /**
   * @brief Initializes the internal timer for frame time calculation.
   * @note This method must be called before using update_frame_time or
   * delta_time.
   */
  void init_frame_timer() override;

  /**
   * @brief Updates the frame time based on the elapsed time since the last
   * frame.
   * @param time_scale A modifier to scale the delta time, useful for
   * implementing slow-motion or speed-up effects.
   */
  void update_frame_time(float time_scale) override;

  /**
   * @brief Retrieves the time elapsed since the last frame.
   * @return The delta time in seconds.
   */
  [[nodiscard]] float delta_time() const override;

  /**
   * @brief Retrieves the frames per second (FPS) based on the updated frame
   * time.
   * @return The current frames per second.
   */
  [[nodiscard]] float frames_per_second() const override;

  /**
   * @brief Gets the current text from the system clipboard.
   * @return The text from the clipboard.
   */
  [[nodiscard]] std::string get_clipboard_text() override;

  /**
   * @brief Sets the cursor to the standard arrow shape.
   */
  void set_cursor_to_arrow() override;

  /**
   * @brief Sets the cursor to the hand shape.
   */
  void set_cursor_to_hand() override;

  /**
   * @brief Sets the cursor to the I-beam shape.
   */
  void set_cursor_to_ibeam() override;

  /**
   * @brief Sets the cursor to the crosshair shape.
   */
  void set_cursor_to_crosshair() override;

  /**
   * @brief Sets the cursor to the wait (hourglass) shape.
   */
  void set_cursor_to_wait() override;

 private:
  std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners_{
      nullptr};
};