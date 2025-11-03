#pragma once

#include <memory>
#include <unordered_map>
#include <optional>
#include <vector>
#include <string>

#include <engine/core/iEngineService.h>
#include <engine/audio/sound/sound_instance.h>
#include <engine/audio/sound/sound_resource.h>

/**
 * @brief Manages audio playback and sound instances within the engine.
 */
class AudioService : public IEngineService {
public:
    AudioService() = default;
    ~AudioService() override = default;

    SoundResource* register_sound(const std::string& file_path, const std::string& name);
    bool unregister_sound(const std::string& name);

    [[nodiscard]] SoundResource* get_sound_resource(const std::string& name) const noexcept;
    [[nodiscard]] SoundInstance* get_sound_instance(const std::string& name) const noexcept;
    [[nodiscard]] std::vector<SoundInstance*> get_all_playing_sounds() const noexcept;

    void play_sound(const SoundResource& sound_resource);
    void play_sound(const std::string& name);

    void stop_sound(const SoundInstance& sound_instance);
    void stop_sound(const std::string& name);
    void stop_all_sounds();

    void set_sound_resource_volume(SoundResource& sound_resource, float volume) noexcept;
    void set_sound_instance_volume(SoundInstance& sound_instance, float volume) noexcept;
    void set_sound_volume(const std::string& name, float volume) noexcept;

    /**
     * @brief Updates the audio manager, meaning it cleans up finished sounds.
     */
    void update();

private:
    std::unordered_map<std::string, std::unique_ptr<SoundResource>> sound_resources;
    std::vector<std::unique_ptr<SoundInstance>> active_instances;
};