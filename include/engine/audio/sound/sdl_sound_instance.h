#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <engine/audio/sound/sdl_sound_resource.h>
#include <engine/audio/sound/sound_instance.h>

/**
 * @brief Represents a SDL-specific implementation of a sound instance.
 * Inherits from the generic SoundInstance class and contains all SDL logic.
 */
class SDLSoundInstance : public SoundInstance {
public:
    SDLSoundInstance(std::shared_ptr<SDLSoundResource> resource, float volume = 1.0f);
    ~SDLSoundInstance() override = default;

    void play() override;
    void pause() override;
    void stop() override;
    void set_volume(float volume) override;
    bool is_playing() const override;

private:
    std::unique_ptr<SDL_AudioStream, decltype(&SDL_DestroyAudioStream)> stream{nullptr, &SDL_DestroyAudioStream};
};