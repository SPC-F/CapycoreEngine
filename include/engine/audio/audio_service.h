#pragma once

#include <memory>
#include <unordered_map>
#include <optional>
#include <vector>
#include <string>

#include <engine/core/iEngineService.h>
#include <engine/audio/sound/sound_instance.h>
#include <engine/audio/sound/sound_resource.h>
#include <engine/audio/sound/sound_type.h>

/**
 * @brief Manages audio playback and sound instances within the engine.
 */
class AudioService : public IEngineService {
public:
    AudioService() = default;
    ~AudioService() override = default;

    std::shared_ptr<SoundResource> register_sound(const std::string& file_path, const std::string& name, SoundType type = SoundType::GENERIC);
    std::shared_ptr<SoundResource> register_sdl_sound(const std::string& file_path, const std::string& name);
    bool unregister_sound(const std::string& name);
    
    [[nodiscard]] std::shared_ptr<SoundResource> get_sound_resource(const std::string& name) const noexcept;

    std::shared_ptr<SoundInstance> play_sound(std::shared_ptr<SoundResource> sound_resource);
    std::shared_ptr<SoundInstance> play_sound(const std::string& name);

    void stop_sound(std::shared_ptr<SoundInstance> sound_instance);
    void stop_sound(const std::string& name);
    void stop_all_sounds();

    [[nodiscard]] std::shared_ptr<SoundInstance> get_sound_instance(const std::string& name) const noexcept;
    [[nodiscard]] std::vector<std::shared_ptr<SoundInstance>> get_all_playing_sounds() const noexcept;


    void set_sound_volume(std::shared_ptr<SoundInstance> sound_instance, float volume) noexcept;
    void set_sound_volume(const std::string& name, float volume) noexcept;

    /**
     * @brief Updates the audio manager, meaning it cleans up finished sounds.
     */
    void update();

private:
    std::unordered_map<std::string, std::shared_ptr<SoundResource>> sound_resources_;
    std::vector<std::shared_ptr<SoundInstance>> active_instances_;
};