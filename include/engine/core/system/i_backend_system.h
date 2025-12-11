#pragma once

#include <engine/core/events/system_event.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using BackendCallback = std::function<void(void* event_data)>;

/**
 * @brief Interface for backend system implementations handling low-level
 * events.
 *
 * Defines methods for initializing event listeners, polling events, and
 * managing frame timing. Implementations should translate backend-specific
 * events into engine-recognized events.
 */
class IBackendSystem {
 public:
  virtual ~IBackendSystem() = default;

  virtual void initialize_listeners(
      std::unordered_map<SystemEvent, std::vector<BackendCallback>>*
          listeners) = 0;

  /**
   * @brief Polls for new events from the backend system.
   *
   * Should invoke registered callbacks for each event type as events are
   * received.
   */
  virtual void poll_events() = 0;

  /**
   * @brief Initializes the internal timer for frame time calculation.
   * @note This method must be called before using update_frame_time or
   * delta_time.
   * @note Techically this keeps track of time, but it utilizes SDL's
   * performance counter under the hood so thats why its here.
   *
   * This method sets up the necessary variables to track frame time and should
   * be called before starting the rendering loop.
   */
  virtual void init_frame_timer() = 0;

  /**
   * @brief Updates the frame time based on the elapsed time since the last
   * frame.
   * @param time_scale A modifier to scale the delta time, useful for
   * implementing slow-motion or speed-up effects.
   */
  virtual void update_frame_time(float time_scale) = 0;

  /**
   * @brief Retrieves the time elapsed since the last frame.
   * @return The delta time in seconds.
   */
  [[nodiscard]] virtual float delta_time() const = 0;

  /**
   * @brief Retrieves the frames per second (FPS) based on the updated frame
   * time.
   * @return The current frames per second.
   */
  [[nodiscard]] virtual float frames_per_second() const = 0;

  /**
   * @brief Sets the current text to the system clipboard.
   * @param text The text to set to the clipboard.
   */
  [[nodiscard]] virtual std::string get_clipboard_text() = 0;

  /**
   * @brief Sets the cursor to the standard arrow shape.
   */
  virtual void set_cursor_to_arrow() = 0;

  /**
   * @brief Sets the cursor to the hand shape.
   */
  virtual void set_cursor_to_hand() = 0;

  /**
   *  @brief Sets the cursor to the I-beam shape.
   */
  virtual void set_cursor_to_ibeam() = 0;

  /**
   * @brief Sets the cursor to the crosshair shape.
   */
  virtual void set_cursor_to_crosshair() = 0;

  /**
   * @brief Sets the cursor to the wait (hourglass) shape.
   */
  virtual void set_cursor_to_wait() = 0;

 protected:
  uint64_t last_{0};
  double freq_{0.0f};
  float delta_time_{0.0f};

  int frame_count_{0};
  float fps_{0.0f};
  float accumulated_time_{0.0f};
};
