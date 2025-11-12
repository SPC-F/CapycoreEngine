#include <engine/audio/sound/resource/sdl_sound_resource.h>

#include <filesystem>

SDLSoundResource::SDLSoundResource(const std::string& name, const std::string& file_path, SoundType type)
    : SoundResource(name, file_path, type) {
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            throw std::runtime_error("Failed to initialize SDL audio subsystem: " + std::string(SDL_GetError()));
        }
    }

    if (file_path.size() < 4 || file_path.substr(file_path.size() - 4) != ".wav") {
        throw std::runtime_error("SDLSoundResource only supports .wav files: " + file_path);
    }

    if (!std::filesystem::exists(file_path)) {
        throw std::runtime_error("WAV file does not exist: " + file_path);
    }
    
    try {
        Uint8* buffer = nullptr;

        if (!SDL_LoadWAV(file_path.c_str(), &spec_, &buffer, &audio_length_)) {
            throw std::runtime_error("Failed to load WAV file: " + std::string(SDL_GetError()));
        }

        audio_buffer_.reset(buffer);
    } catch (...) {
        throw;
    }
}

const SDL_AudioSpec& SDLSoundResource::audio_spec() const {
    return spec_;
}

const Uint8* SDLSoundResource::audio_buffer() const {
    return audio_buffer_.get();
}

Uint32 SDLSoundResource::length() const {
    return audio_length_;
}