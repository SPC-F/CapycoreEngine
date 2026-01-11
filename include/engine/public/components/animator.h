#pragma once

#include <engine/core/rendering/texture.h>
#include <engine/public/component.h>
#include <engine/public/gameObject.h>

#include <vector>

/**
 * @brief Component that handles sprite sheet animations.
 *
 * The Animator component allows you to create frame-by-frame animations
 * using a sprite sheet. You can control playback, looping, and frame timing.
 *
 * Usage:
 * - Create an Animator with a sprite sheet and frame interval.
 * - Use play(), pause(), and reset() to control animation playback.
 * - Set specific animations using set_animation().
 * - The component updates the sprite texture based on the current frame.
 * @note Ensure the parent GameObject has a Sprite component to display frames.
 */
class Animator : public Component {
 private:
  std::vector<std::reference_wrapper<Texture>> frames_;
  const int interval_ms_;

  int current_texture_index_;
  int accumulator_time_ms_;

  bool is_playing_;
  bool is_looping_;
  bool is_non_interruptible_{false};

  [[nodiscard]] int calculate_next_frame_index(int intervals_advanced) const;
  void update_sprite_texture(int new_frame_index);

 public:
  explicit Animator(const std::string& sprite_sheet_name, int interval_ms);

  void play(bool is_looping);
  void play(const std::string& animation_name, bool is_looping);

  void pause();
  void reset();
  [[nodiscard]] bool is_playing() const noexcept;

  [[nodiscard]] bool is_non_interruptible() const noexcept;
  void is_non_interruptible(bool value) noexcept;

  // Component overrides
  void update(float dt_seconds) override;

  void set_animation(const std::string& animation_name);
  void set_animation(
      const std::vector<std::reference_wrapper<Texture>>& frames_);

  std::string type_name() const override;
};