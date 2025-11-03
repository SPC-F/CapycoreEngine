#include <engine/audio/audio_service.h>

#include <algorithm>

#include <engine/audio/sound/sound_factory.h>

std::shared_ptr<SoundResource> AudioService::register_sound(const std::string& file_path, const std::string& name, SoundType type) {
    auto it = sound_resources_.find(name);
    
    if (it != sound_resources_.end()) {
        return it->second;
    }

    auto resource = SoundFactory::create_sound_resource(file_path, name, type);
    sound_resources_[name] = resource;
    return resource;
}

std::shared_ptr<SoundResource> AudioService::register_sdl_sound(const std::string& file_path, const std::string& name) {
    return register_sound(file_path, name, SoundType::SDL_MIXER);
}

bool AudioService::unregister_sound(const std::string& name) {
    return sound_resources_.erase(name) > 0;
}

std::shared_ptr<SoundResource> AudioService::get_sound_resource(const std::string& name) const noexcept {
    auto it = sound_resources_.find(name);
    
    if (it != sound_resources_.end()) {
        return it->second;
    }

    return nullptr;
}

std::shared_ptr<SoundInstance> AudioService::play_sound(std::shared_ptr<SoundResource> sound_resource) {
    if (!sound_resource) {
        return nullptr;
    }

    auto instance = SoundFactory::create_sound_instance(sound_resource);
    instance->play();
    active_instances_.push_back(instance);

    return instance;
}

std::shared_ptr<SoundInstance> AudioService::play_sound(const std::string& name) {
    return play_sound(get_sound_resource(name));
}

void AudioService::stop_sound(std::shared_ptr<SoundInstance> sound_instance) {
    if (!sound_instance) {
        return;
    }

    sound_instance->stop();

    active_instances_.erase(
        std::remove(active_instances_.begin(), active_instances_.end(), sound_instance),
        active_instances_.end()
    );
}

void AudioService::stop_sound(const std::string& name) {
    stop_sound(get_sound_instance(name));
}

void AudioService::stop_all_sounds() {
    for (auto& instance : active_instances_) {
        instance->stop();
    }

    active_instances_.clear();
}

std::shared_ptr<SoundInstance> AudioService::get_sound_instance(const std::string& name) const noexcept {
    auto it = std::find_if(
        active_instances_.begin(), active_instances_.end(),
        [&name](const std::shared_ptr<SoundInstance>& instance) {
            return instance->resource()->name() == name;
        }
    );

    if (it != active_instances_.end()) {
        return *it;
    }

    return nullptr;
}

std::vector<std::shared_ptr<SoundInstance>> AudioService::get_all_playing_sounds() const noexcept {
    std::vector<std::shared_ptr<SoundInstance>> playing_sounds;
    
    for (const auto& instance : active_instances_) {
        if (instance->is_playing()) {
            playing_sounds.push_back(instance);
        }
    }

    return playing_sounds;
}

void AudioService::set_sound_volume(std::shared_ptr<SoundInstance> sound_instance, float volume) noexcept {
    if (!sound_instance) {
        return;
    }

    sound_instance->set_volume(volume);
}

void AudioService::set_sound_volume(const std::string& name, float volume) noexcept {
    set_sound_volume(get_sound_instance(name), volume);
}

void AudioService::update() {
    active_instances_.erase(
        std::remove_if(
            active_instances_.begin(), active_instances_.end(),
            [](const std::shared_ptr<SoundInstance>& instance) {
                return !instance->is_playing();
            }
        ),
        active_instances_.end()
    );
}
