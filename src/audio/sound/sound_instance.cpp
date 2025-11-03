#include <engine/audio/sound/sound_instance.h>

SoundInstance::SoundInstance(std::shared_ptr<SoundResource> resource, float volume)
    : resource_(resource), volume_(volume) {}

void SoundInstance::set_volume(float volume) {
    volume_ = volume;
}

bool SoundInstance::is_playing() const {
    return playing_;
}

const std::shared_ptr<SoundResource>& SoundInstance::resource() const noexcept {
    return resource_;
}

const float SoundInstance::volume() const noexcept {
    return volume_;
}

void SoundInstance::volume(float volume) noexcept {
    volume_ = volume;
}