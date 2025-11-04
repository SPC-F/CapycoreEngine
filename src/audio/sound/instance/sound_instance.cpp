#include <engine/audio/sound/instance/sound_instance.h>

SoundInstance::SoundInstance(std::shared_ptr<SoundResource> resource, float volume)
    : resource_(resource), volume_(volume) {}

bool SoundInstance::is_playing() const noexcept {
    return playing_ && !paused_;
}

bool SoundInstance::is_paused() const noexcept {
    return paused_;
}

bool SoundInstance::is_finished() {
    return finished_;
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

const bool SoundInstance::finished() const noexcept {
    return finished_;
}

void SoundInstance::finished(bool finished) noexcept {
    finished_ = finished;
}

const bool SoundInstance::loop() const noexcept {
    return loop_;
}

void SoundInstance::loop(bool loop) noexcept {
    loop_ = loop;
}