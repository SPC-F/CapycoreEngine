#include <engine/audio/sound/sdl_sound_instance.h>

SDLSoundInstance::SDLSoundInstance(const std::string& name, const std::string& file_path, float volume)
    : SoundInstance(name, file_path, volume) {
    //
}

void SDLSoundInstance::play() {
    // 
}

void SDLSoundInstance::pause() {
    // 
}

void SDLSoundInstance::stop() {
    // 
}

void SDLSoundInstance::setVolume(float volume) {
    // 
}

bool SDLSoundInstance::isPlaying() const {
    return playing;
}

