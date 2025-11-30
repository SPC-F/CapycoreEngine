#pragma once

#include <engine/audio/sound/instance/sound_instance.h>
#include <engine/audio/sound/resource/sound_resource.h>
#include <engine/audio/sound/sound_type.h>
#include <engine/core/iEngineService.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Manages audio playback and sound instances within the engine.
 */
class AudioService : public IEngineService {
 public:
  AudioService() = default;

  std::optional<std::shared_ptr<SoundResource>> register_sound(
      const std::string& file_path, const std::string& name,
      SoundType type = SoundType::GENERIC);
  bool unregister_sound(const std::string& name);

  std::optional<std::shared_ptr<SoundResource>> get_sound_resource(
      const std::string& file_path,
      const std::string& name = "") const noexcept;

  std::reference_wrapper<SoundInstance> play_sound(
      std::shared_ptr<SoundResource> sound_resource, float volume = 1.0f,
      bool loop = false);
  std::reference_wrapper<SoundInstance> play_sound(const std::string& name,
                                                   float volume = 1.0f,
                                                   bool loop = false);

  void stop_sound(SoundInstance& instance);
  void stop_sound(const std::string& name);
  void stop_all_sounds();

  bool has_sound_instance(const std::string& name) const noexcept;
  std::optional<std::reference_wrapper<SoundInstance>> get_sound_instance(
      const std::string& name) const;
  std::vector<std::reference_wrapper<SoundInstance>> get_all_playing_sounds()
      const;

  /**
   * @brief Updates the audio manager, meaning it cleans up finished sounds.
   */
  void update();

 private:
  std::unordered_map<std::string, std::shared_ptr<SoundResource>>
      sound_resources_;
  std::vector<std::unique_ptr<SoundInstance>> active_instances_;
};