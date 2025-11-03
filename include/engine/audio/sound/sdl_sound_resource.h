#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <engine/audio/sound/sound_resource.h>

/**
 * @brief Represents a SDL-specific resource.
 * Inherits from the generic SoundResource class and contains SDL-specific audio data.
 */
class SDLSoundResource : public SoundResource {
public:
    SDLSoundResource(const std::string& name, const std::string& file_path, float volume = 1.0f);
    ~SDLSoundResource() override = default;

    const SDL_AudioSpec& getAudioSpec() const;
    const Uint8* getAudioBuffer() const;
    Uint32 getDurationInMs() const;

private:
    SDL_AudioSpec spec;
    Uint32 duration_in_ms{0};
    std::unique_ptr<Uint8, decltype(&SDL_free)> audio_buffer{nullptr, &SDL_free};
};