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
    explicit SoundInstance(SoundResource* resource, float volume = 1.0f)
        : resource_(resource), volume_(volume) {}
    virtual ~SoundInstance() = default;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void set_volume(float volume) = 0;
    virtual bool is_playing() const = 0;

protected:
    SoundResource* resource_;
    float volume_{1.0f};
    bool playing_{false};
};