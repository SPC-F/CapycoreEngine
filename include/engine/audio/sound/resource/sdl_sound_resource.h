#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <engine/audio/sound/resource/sound_resource.h>
#include <engine/audio/sound/sound_type.h>

/**
 * @brief Represents a SDL-specific resource.
 * Inherits from the generic SoundResource class and contains SDL-specific audio data.
 */
class SDLSoundResource : public SoundResource {
public:
    SDLSoundResource(const std::string& name, const std::string& file_path, SoundType type = SoundType::SDL_MIXER);
    ~SDLSoundResource() override = default;

    [[nodiscard]]
    const SDL_AudioSpec& audio_spec() const;
    [[nodiscard]]
    const Uint8* audio_buffer() const;
    [[nodiscard]]
    Uint32 length() const;

private:
    SDL_AudioSpec spec_{};
    Uint32 audio_length_{0};
    std::unique_ptr<Uint8, decltype(&SDL_free)> audio_buffer_{nullptr, &SDL_free};
};