#include <engine/audio/sound/instance/sdl_sound_instance.h>

#include <algorithm>
#include <vector>

constexpr float sample_clamp_range = 32768.0f;

SDLSoundInstance::SDLSoundInstance(std::shared_ptr<SoundResource> resource, float volume)
    : SoundInstance(std::move(resource), volume) {
    // The instance already has the sound buffer and SDL spec from the resource
}

SDLSoundInstance::~SDLSoundInstance() {
    SDL_PauseAudioStreamDevice(stream_.get());
    SDL_ClearAudioStream(stream_.get());
    stream_.reset();
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
        auto* samples = reinterpret_cast<Sint16*>(temp.data());
        size_t count = length / sizeof(Sint16);

        for (size_t i = 0; i < count; ++i) {
            float scaled = static_cast<float>(samples[i]) * volume_;
            samples[i] = static_cast<Sint16>(std::clamp(scaled, -sample_clamp_range, sample_clamp_range - 1));
        }
    }

    stream_.reset(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr));
    if (!stream_) {
        throw std::runtime_error("Failed to open SDL audio stream.");
    }

    SDL_ResumeAudioStreamDevice(stream_.get());
    SDL_PutAudioStreamData(stream_.get(), temp.data(), static_cast<int>(temp.size()));
    
    playing_ = true;
    paused_ = false;
}

void SDLSoundInstance::pause() {
    if (!paused_) {
        return;
    }

    SDL_PauseAudioStreamDevice(stream_.get());
    playing_ = false;
    paused_ = true;
}

void SDLSoundInstance::resume() {
    if (!paused_) {
        return;
    }

    SDL_ResumeAudioStreamDevice(stream_.get());
    playing_ = true;
    paused_ = false;
}

void SDLSoundInstance::stop() {
    if (!playing_) {
        return;
    }

    SDL_PauseAudioStreamDevice(stream_.get());
    SDL_ClearAudioStream(stream_.get());
    stream_.reset();

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
    if (!stream_ || !playing_) {
        return finished_;
    }

    if (SDL_GetAudioStreamAvailable(stream_.get()) > 0) {
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