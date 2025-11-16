#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <engine/audio/sound/resource/sdl_sound_resource.h>
#include <engine/audio/sound/instance/sound_instance.h>

/**
 * @brief Represents a SDL-specific implementation of a sound instance.
 * Inherits from the generic SoundInstance class and contains all SDL logic.
 */
class SDLSoundInstance : public SoundInstance {
public:
    SDLSoundInstance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
    ~SDLSoundInstance() override;

    void play() override;
    void pause() override;
    void resume() override;
    void stop() override;
    
    [[nodiscard]]
    bool is_playing() const noexcept override;
    [[nodiscard]]
    bool is_paused() const noexcept override;
    [[nodiscard]]
    bool is_finished() override;

private:
    std::unique_ptr<SDL_AudioStream, decltype(&SDL_DestroyAudioStream)> stream_{nullptr, &SDL_DestroyAudioStream};
};