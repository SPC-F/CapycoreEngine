#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <engine/audio/sound/sound_instance.h>

/**
 * @brief Represents a SDL-specific implementation of a sound instance.
 */
struct SDLSoundInstance : public SoundInstance {
    SDLSoundInstance() = default;
    
    SDL_AudioSpec spec;
    Uint32 duration_in_ms{0};
    std::unique_ptr<Uint8, decltype(&SDL_free)> audio_buffer{nullptr, &SDL_free};
    std::unique_ptr<SDL_AudioStream, decltype(&SDL_DestroyAudioStream)> audio_stream{nullptr, &SDL_DestroyAudioStream};

    ~SDLSoundInstance() override = default;
};