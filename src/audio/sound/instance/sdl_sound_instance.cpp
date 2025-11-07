#include <engine/audio/sound/instance/sdl_sound_instance.h>

#include <algorithm>
#include <vector>

SDLSoundInstance::SDLSoundInstance(std::shared_ptr<SDLSoundResource> resource, float volume)
    : SoundInstance(resource, volume) {
    // The instance already has the sound buffer and SDL spec from the resource
}

SDLSoundInstance::~SDLSoundInstance() {
    stop();
}

void SDLSoundInstance::play() {
    if (playing_) {
        return;
    }

    auto sdl_resource = std::dynamic_pointer_cast<SDLSoundResource>(resource_);
    auto spec = sdl_resource->audio_spec();
    const Uint8* src_buffer = sdl_resource->audio_buffer();
    Uint32 length = sdl_resource->length();

    std::vector<Uint8> temp(src_buffer, src_buffer + length);

    if (spec.format == SDL_AUDIO_S16LE || spec.format == SDL_AUDIO_S16BE) {
        Sint16* samples = reinterpret_cast<Sint16*>(temp.data());
        size_t count = length / sizeof(Sint16);

        for (size_t i = 0; i < count; ++i) {
            float scaled = static_cast<float>(samples[i]) * volume_;
            samples[i] = static_cast<Sint16>(std::clamp(scaled, -32768.0f, 32767.0f));
        }
    }

    stream.reset(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr));
    if (!stream) {
        throw std::runtime_error("Failed to open SDL audio stream.");
    }

    SDL_ResumeAudioStreamDevice(stream.get());
    SDL_PutAudioStreamData(stream.get(), temp.data(), static_cast<int>(temp.size()));
    
    playing_ = true;
    paused_ = false;
}

void SDLSoundInstance::pause() {
    if (!paused_) {
        return;
    }

    SDL_PauseAudioStreamDevice(stream.get());
    playing_ = false;
    paused_ = true;
}

void SDLSoundInstance::resume() {
    if (!paused_) {
        return;
    }

    SDL_ResumeAudioStreamDevice(stream.get());
    playing_ = true;
    paused_ = false;
}

void SDLSoundInstance::stop() {
    if (!playing_) {
        return;
    }

    SDL_PauseAudioStreamDevice(stream.get());
    SDL_ClearAudioStream(stream.get());
    stream.reset();

    playing_ = false;
    paused_ = false;
    finished_ = true;
}

bool SDLSoundInstance::is_playing() const noexcept {
    return playing_ && !paused_;
}

bool SDLSoundInstance::is_paused() const noexcept {
    return paused_;
}

bool SDLSoundInstance::is_finished() {
    if (!stream || !playing_) {
        return finished_;
    }

    if (SDL_GetAudioStreamAvailable(stream.get()) > 0) {
        return false;
    }

    stop();

    if (loop_) {
        finished_ = false;
        play();
        return false;
    }

    finished_ = true;
    return true;
}