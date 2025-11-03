#include <engine/audio/sound/sdl_sound_instance.h>

SDLSoundInstance::SDLSoundInstance(SDLSoundResource* resource, float volume)
    : SoundInstance(resource, volume) {
    // Initialize SDL AudioStream here using the SoundResource data
}

void SDLSoundInstance::play() {
    // Implement play logic using SDL
}

void SDLSoundInstance::pause() {
    // Implement pause logic using SDL
}

void SDLSoundInstance::stop() {
    // Implement stop logic using SDL
}

void SDLSoundInstance::set_volume(float volume) {
    volume_ = volume;
    // Implement volume adjustment logic using SDL
}

bool SDLSoundInstance::is_playing() const {
    return playing_;
}
