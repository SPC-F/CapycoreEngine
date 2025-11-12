#pragma once

#include <memory>
#include <optional>
#include <string>

#include <engine/public/component.h>
#include <engine/audio/sound/resource/sound_resource.h>
#include <engine/audio/sound/instance/sound_instance.h>

/**
 * @brief Component that plays audio using the AudioService.
 * 
 * AudioSource allows a GameObject to play sounds, with options for
 * looping, volume control, and play-on-awake functionality.
 * 
 * @param audio_file_path The file path to the audio clip.
 * @param play_on_awake Whether the audio should play automatically when the component is attached.
 * @param loop Whether the audio should loop after finishing.
 * @param volume The volume level of the audio (0.0 to 1.0).
 */
class AudioSource : public Component
{
public:
    explicit AudioSource(std::string audio_file_path, bool play_on_awake = true, bool loop = false, float volume = 1.0f);
    ~AudioSource() override = default;

    void update(float dt) override;
    void on_attach() override;
    void on_detach() override;
    void on_serialize() override;
    void on_deserialize() override;
    
    void play(bool loop = false);
    void stop();

    [[nodiscard]]
    const std::string& audio_file_path() const noexcept;
    void audio_file_path(const std::string& path) noexcept;

    [[nodiscard]]
    float volume() const noexcept;
    void volume(float value) noexcept;

    [[nodiscard]]
    bool loop() const noexcept;
    void loop(bool value) noexcept;

    [[nodiscard]]
    bool play_on_awake() const noexcept;
    void play_on_awake(bool value) noexcept;

private:
    std::shared_ptr<SoundResource> get_or_register_resource();
    std::optional<std::reference_wrapper<SoundInstance>> instance_opt_;

    std::string audio_file_path_;
    std::string audio_name_;
    float volume_;

    bool loop_;
    bool play_on_awake_;
};