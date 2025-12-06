#include <engine/core/engine.h>
#include <engine/core/rendering/assetService.h>
#include <engine/public/components/animator.h>
#include <engine/public/components/sprite.h>

std::vector<std::reference_wrapper<Texture>> get_frames(
    const std::string& sprite_sheet_name) {
  const auto& service =
      Engine::instance().services->get_service<AssetService>().get();
  const auto sprite_sheet = service.try_get_spritesheet(sprite_sheet_name);

  if (!sprite_sheet.has_value()) {
    throw std::runtime_error("Animator: Sprite sheet not found: " +
                             sprite_sheet_name);
  }

  return sprite_sheet->get();
}

Animator::Animator(const std::string& sprite_sheet_name, const int interval_ms)
    : frames_(get_frames(sprite_sheet_name)),
      interval_ms_(interval_ms > 0
                       ? interval_ms
                       : throw std::invalid_argument(
                             "Animator: interval_ms must be greater than 0")),
      current_texture_index_(0),
      accumulator_time_ms_(0),
      is_playing_(false),
      is_looping_(true) {}

void Animator::play(const bool is_looping) {
  is_playing_ = true;
  is_looping_ = is_looping;
}

void Animator::pause() { is_playing_ = false; }

void Animator::reset() {
  accumulator_time_ms_ = 0;
  current_texture_index_ = 0;
}

bool Animator::is_playing() const noexcept { return is_playing_; }

/**
 * So the C++ modulo works crappy with negative numbers.
 * Imagine current_frame_index = 2, frame_count = 5, frames_advanced = -3
 * (2 + -3) = -1, -1 % 5 = -1 (what we do not want, invalid index)
 * Add the frame count to make it positive: -1 + 5 = 4
 * 4 % 5 = 4 (what we want)
 * ((2 + -3) % 5 + 5) % 5 = (-1 + 5) % 5 = 4 % 5 = 4 (correct)
 * This allows us to keep the code 'simple' ¯\_(ツ)_/¯.
 */
static int mathematically_correct_modulo(int value, const int base) {
  value = value % base;
  if (value < 0) {
    value += base;
  }
  return value;
}

int Animator::calculate_next_frame_index(const int intervals_advanced) const {
  const int animation_size = static_cast<int>(frames_.size());

  if (current_texture_index_ < 0 || current_texture_index_ >= animation_size) {
    throw std::out_of_range(
        "next_frame_index: current_frame_index out of range");
  }

  if (intervals_advanced == 0) {
    return current_texture_index_;
  }

  if (!is_looping_) {
    const int new_frame_index = current_texture_index_ + intervals_advanced;
    if (new_frame_index < 0) {
      return 0;
    }
    return std::min(new_frame_index, animation_size - 1);
  }

  return mathematically_correct_modulo(
      current_texture_index_ + intervals_advanced, animation_size);
}

void Animator::update_sprite_texture(const int new_frame_index) {
  const auto& parent_opt = this->parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error("Animator: No parent GameObject found.");
  }
  const GameObject& parent = parent_opt.value().get();
  const auto sprite = parent.get_component<Sprite>();

  if (!sprite.has_value()) {
    throw std::runtime_error(
        "Animator: Parent GameObject does not have a Sprite component.");
  }

  sprite->get().texture(frames_[new_frame_index]);
}

// Component overrides
void Animator::update(const float dt_seconds) {
  if (!is_playing_ || frames_.empty() || interval_ms_ <= 0) {
    return;
  }

  const int actual_dt_ms =
      static_cast<int>(dt_seconds * 1000.0f) + accumulator_time_ms_;

  if (actual_dt_ms < interval_ms_) {
    this->accumulator_time_ms_ = static_cast<int>(actual_dt_ms);
    return;
  }

  this->accumulator_time_ms_ = actual_dt_ms % interval_ms_;

  // How many frames have we advanced? Take of the excess time if any, be left
  // with only an exact number of frames.
  const int intervals_advanced =
      static_cast<int>((actual_dt_ms - accumulator_time_ms_) / interval_ms_);
  const int new_frame_index = calculate_next_frame_index(intervals_advanced);

  update_sprite_texture(new_frame_index);
  current_texture_index_ = new_frame_index;

  if (!is_looping_ && new_frame_index == frames_.size() - 1) {
    is_playing_ = false;
  }
}

void Animator::on_serialize() {}

void Animator::on_deserialize() {}

void Animator::set_animation(
    const std::vector<std::reference_wrapper<Texture>>& frames) {

  if (frames.empty()) {
    throw std::invalid_argument("Animator: frames cannot be empty.");
  }

  frames_ = frames;
  reset();
}
