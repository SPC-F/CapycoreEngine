#pragma once

#include <memory>
#include <string>

#include <engine/audio/sound/resource/sound_resource.h>

/**
 * @brief Represents an instance of a generic sound within the audio system.
 * The reason for separating SoundInstance from SoundResource is to allow multiple
 * instances of the same sound resource to be played simultaneously with different
 * settings (e.g., volume, loop).
 */
class SoundInstance {
public:
    explicit SoundInstance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
    virtual ~SoundInstance() = default;

    virtual void play() {}
    virtual void pause() {}
    virtual void resume() {}
    virtual void stop() {}

    virtual bool is_playing() const noexcept;
    virtual bool is_paused() const noexcept;
    virtual bool is_finished();

    const std::shared_ptr<SoundResource>& resource() const noexcept;
    const float volume() const noexcept;
    void volume(float volume) noexcept;

    const bool finished() const noexcept;
    void finished(bool finished) noexcept;

    const bool loop() const noexcept;
    void loop(bool loop) noexcept;
    
protected:
    std::shared_ptr<SoundResource> resource_;

    bool loop_{false};
    bool playing_{false};
    bool paused_{false};
    bool finished_{false};
    float volume_{1.0f};
};