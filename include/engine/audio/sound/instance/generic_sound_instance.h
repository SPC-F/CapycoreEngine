#pragma once

#include <memory>

#include <engine/audio/sound/instance/sound_instance.h>

/**
 * @brief A generic implementation of a sound instance.
 * Provides the basic functionality while not tied to any specific audio backend.
 */
class GenericSoundInstance : public SoundInstance {
public:
    GenericSoundInstance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
    ~GenericSoundInstance() override = default;

    void play() override;
    void pause() override;
    void resume() override;
    void stop() override;
    
    bool is_playing() const noexcept override;
    bool is_paused() const noexcept override;
    bool is_finished() override;
};
