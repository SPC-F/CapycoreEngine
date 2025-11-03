#include <engine/audio/sound/sdl_sound_resource.h>

SDLSoundResource::SDLSoundResource(const std::string& name, const std::string& file_path, float volume)
    : SoundResource(name, file_path, volume) {
    // Load WAV file
}

const SDL_AudioSpec& SDLSoundResource::getAudioSpec() const {
    return spec;
}

const Uint8* SDLSoundResource::getAudioBuffer() const {
    return audio_buffer.get();
}

Uint32 SDLSoundResource::getDurationInMs() const {
    return duration_in_ms;
}