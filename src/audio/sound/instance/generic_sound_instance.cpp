#include <engine/audio/sound/instance/generic_sound_instance.h>

GenericSoundInstance::GenericSoundInstance(std::shared_ptr<SoundResource> resource, float volume)
    : SoundInstance(std::move(resource), volume) {}

void GenericSoundInstance::play() {
    playing_ = true;
}

void GenericSoundInstance::pause() {
    playing_ = false;
    paused_ = true;
}

void GenericSoundInstance::resume() {
    playing_ = true;
    paused_ = false;
}

void GenericSoundInstance::stop() {
    playing_ = false;
    paused_ = false;
    finished_ = true;
}

bool GenericSoundInstance::is_playing() const noexcept {
    return playing_ && !paused_;
}

bool GenericSoundInstance::is_paused() const noexcept {
    return paused_;
}

bool GenericSoundInstance::is_finished() {
    return !playing_;
}