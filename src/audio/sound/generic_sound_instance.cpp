#include <engine/audio/sound/generic_sound_instance.h>

GenericSoundInstance::GenericSoundInstance(std::shared_ptr<SoundResource> resource, float volume)
    : SoundInstance(resource, volume) {}

void GenericSoundInstance::play() {
    playing_ = true;
}

void GenericSoundInstance::pause() {
    playing_ = false;
}

void GenericSoundInstance::stop() {
    playing_ = false;
}

void GenericSoundInstance::set_volume(float volume) {
    volume_ = volume;
}

bool GenericSoundInstance::is_playing() const {
    return playing_;
}