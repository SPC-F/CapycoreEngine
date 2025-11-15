#include <engine/public/components/audio_source.h>

#include <algorithm>
#include <filesystem>

#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>

AudioSource::AudioSource(std::string audio_file_path, bool play_on_awake, bool loop, float volume)
    : audio_file_path_(std::move(audio_file_path)), play_on_awake_(play_on_awake), loop_(loop), volume_(std::clamp(volume, 0.0f, 1.0f))
{
}

void AudioSource::update(float dt)
{
    // Audio's don't need to update as the audio services handles playback
}

void AudioSource::on_attach()
{
    if (play_on_awake_) {
        play(loop_);
    }
}

void AudioSource::on_detach()
{
    stop();
}

void AudioSource::on_serialize()
{
    // TODO: after networking
}

void AudioSource::on_deserialize()
{
    // TODO: after networking
}

std::shared_ptr<SoundResource> AudioSource::get_or_register_resource()
{
    auto& audio_service = Engine::instance().services->get_service<AudioService>().get();

    std::filesystem::path path(audio_file_path_);
    std::string filename = path.filename().string();
    audio_name_ = filename + "_" + std::to_string(reinterpret_cast<uintptr_t>(this));

    auto resource_opt = audio_service.get_sound_resource(audio_file_path_, audio_name_);
    if (!resource_opt) {
        resource_opt = audio_service.register_sound(audio_file_path_, audio_name_, SoundType::SDL_MIXER);
    }

    if (resource_opt.has_value())
    {
        auto name = resource_opt.value()->name();
        if (name != audio_name_) {
            audio_name_ = name;
        }
    }

    return resource_opt.value_or(nullptr);
}

void AudioSource::play(bool loop)
{
    auto& audio_service = Engine::instance().services->get_service<AudioService>().get();
    auto resource = get_or_register_resource();

    if (!resource) {
        throw std::runtime_error("Failed to get or register sound resource for AudioSource");
    }

    if (!instance_opt_) {
        instance_opt_ = audio_service.play_sound(resource, volume_, loop);
        return;
    }

    auto& instance = instance_opt_.value().get();

    if (instance.is_paused()) {
        instance.resume();
        return;
    }

    if (!instance.is_playing()) {
        instance.play();
        return;
    }
}

void AudioSource::stop()
{
    if (!instance_opt_) {
        return;
    }

    auto& audio_service = Engine::instance().services->get_service<AudioService>().get();
    auto& instance = instance_opt_->get();

    audio_service.stop_sound(instance);
    instance_opt_ = std::nullopt;
}

const std::string& AudioSource::audio_file_path() const noexcept
{
    return audio_file_path_;
}

void AudioSource::audio_file_path(const std::string& path) noexcept
{
    audio_file_path_ = path;
}

float AudioSource::volume() const noexcept
{
    return volume_;
}

void AudioSource::volume(float value) noexcept
{
    volume_ = value;
}

bool AudioSource::loop() const noexcept
{
    return loop_;
}

void AudioSource::loop(bool value) noexcept
{
    loop_ = value;
}

bool AudioSource::play_on_awake() const noexcept
{
    return play_on_awake_;
}

void AudioSource::play_on_awake(bool value) noexcept
{
    play_on_awake_ = value;
}

std::string AudioSource::audio_name() const noexcept
{
    return audio_name_;
}

std::optional<std::reference_wrapper<SoundInstance>> AudioSource::instance() const noexcept
{
    return instance_opt_;
}