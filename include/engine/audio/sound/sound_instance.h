#pragma once

#include <memory>
#include <string>

#include <engine/audio/sound/sound_resource.h>

/**
 * @brief Represents an instance of a generic sound within the audio system.
 * The reason for separating SoundInstance from SoundResource is to allow multiple
 * instances of the same sound resource to be played simultaneously with different
 * settings (e.g., volume, pitch).
 */
class SoundInstance {
public:
    explicit SoundInstance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
    virtual ~SoundInstance() = default;

    virtual void play() {}
    virtual void pause() {}
    virtual void stop() {}
    virtual void set_volume(float volume);
    virtual bool is_playing() const;

    const std::shared_ptr<SoundResource>& resource() const noexcept;
    const float volume() const noexcept;
    void volume(float volume) noexcept;
    
protected:
    std::shared_ptr<SoundResource> resource_;

    int loops_{0};
    bool playing_{false};
    float volume_{1.0f};
};