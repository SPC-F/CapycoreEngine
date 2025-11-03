#pragma once

#include <memory>

#include <engine/audio/sound/sound_instance.h>

class GenericSoundInstance : public SoundInstance {
public:
    GenericSoundInstance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
    ~GenericSoundInstance() override = default;

    void play() override;
    void pause() override;
    void stop() override;
    void set_volume(float volume) override;
    bool is_playing() const override;
};
