#pragma once

#include <memory>
#include <unordered_map>
#include <optional>
#include <vector>
#include <string>

#include <engine/audio/sound/sound_instance.h>
#include <engine/core/iEngineService.h>

/**
 * @brief Manages audio playback and sound instances within the engine.
 */
class AudioManager : public IEngineService {
public:
    AudioManager() = default;
    ~AudioManager() override = default;

    SoundInstance* register_sound(const std::string& file_path, const std::string& name);
    bool unregister_sound(const std::string& name);

    [[nodiscard]] SoundInstance* get_sound(const std::string& name) const noexcept;
    [[nodiscard]] std::vector<SoundInstance*> get_all_sounds() const noexcept;
    [[nodiscard]] std::vector<SoundInstance*> get_all_playing_sounds() const noexcept;

    void play_sound(const SoundInstance& sound_instance);
    void play_sound(const std::string& name);

    void stop_sound(const SoundInstance& sound_instance);
    void stop_sound(const std::string& name);
    void stop_all_sounds();

    void set_sound_volume(SoundInstance& sound_instance, float volume) noexcept;
    void set_sound_volume(const std::string& name, float volume) noexcept;

    /**
     * @brief Updates the audio manager, meaning it cleans up finished sounds.
     */
    void update();

private:
    std::unordered_map<std::string, std::unique_ptr<SoundInstance>> sound_instances_;
    std::vector<SoundInstance*> playing_sounds;
};