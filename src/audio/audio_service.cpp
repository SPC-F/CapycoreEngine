#include <engine/audio/audio_service.h>

#include <algorithm>

#include <engine/audio/sound/sound_factory.h>

std::shared_ptr<SoundResource> AudioService::register_sound(const std::string& file_path, const std::string& name, SoundType type) {
    auto it = sound_resources_.find(name);

    if (it != sound_resources_.end()) {
        return it->second;
    }

    try {
        auto resource = SoundFactory::create_sound_resource(file_path, name, type);
        sound_resources_[name] = resource;

        return resource;
    } catch (...) {
        return nullptr;
    }
}

bool AudioService::unregister_sound(const std::string& name) {
    active_instances_.erase(
        std::remove_if(
            active_instances_.begin(), active_instances_.end(),
            [&name](const std::unique_ptr<SoundInstance>& instance) {
                return instance && instance->resource() && (instance->resource()->name() == name);
            }),
        active_instances_.end()
    );

    return sound_resources_.erase(name) > 0;
}

std::shared_ptr<SoundResource> AudioService::get_sound_resource(const std::string& name) const noexcept {
    auto it = sound_resources_.find(name);

    if (it != sound_resources_.end()) {
        return it->second;
    }

    return nullptr;
}

std::reference_wrapper<SoundInstance> AudioService::play_sound(std::shared_ptr<SoundResource> sound_resource, float volume, bool loop) {
    if (!sound_resource) {
        throw std::invalid_argument("Invalid sound resource");
    }

    auto instance = SoundFactory::create_sound_instance(sound_resource, volume);
    auto ref = std::ref(*instance);

    if (!instance) {
        throw std::runtime_error("Failed to create sound instance");
    }

    instance->loop(loop);
    instance->play();

    active_instances_.push_back(std::move(instance));

    return ref;
}

std::reference_wrapper<SoundInstance> AudioService::play_sound(const std::string& name, float volume, bool loop) {
    return play_sound(get_sound_resource(name), volume, loop);
}

void AudioService::stop_sound(std::unique_ptr<SoundInstance>&& sound_instance) {
    if (!sound_instance) {
        return;
    }

    sound_instance->stop();

    active_instances_.erase(
        std::remove_if(
            active_instances_.begin(), active_instances_.end(),
            [&sound_instance](const std::unique_ptr<SoundInstance>& inst) {
                return inst.get() == sound_instance.get();
            }),
        active_instances_.end()
    );

    sound_instance.reset();
}

void AudioService::stop_sound(const std::string& name) {
    auto it = std::find_if(
        active_instances_.begin(), active_instances_.end(),
        [&name](const std::unique_ptr<SoundInstance>& instance) {
            return instance && instance->resource() && (instance->resource()->name() == name);
        }
    );

    if (it != active_instances_.end()) {
        (*it)->stop();
        active_instances_.erase(it);
    }
}

void AudioService::stop_all_sounds() {
    for (auto& instance : active_instances_) {
        if (instance) 
        {
            instance->stop();
        }
    }

    active_instances_.clear();
}

bool AudioService::has_sound_instance(const std::string& name) const noexcept {
    return std::any_of(
        active_instances_.begin(), active_instances_.end(),
        [&name](const std::unique_ptr<SoundInstance>& instance) {
            return instance && instance->resource() && (instance->resource()->name() == name);
        }
    );
}

std::optional<std::reference_wrapper<SoundInstance>> AudioService::get_sound_instance(const std::string& name) const {
    auto it = std::find_if(
        active_instances_.begin(), active_instances_.end(),
        [&name](const std::unique_ptr<SoundInstance>& instance) {
            return instance && instance->resource() && (instance->resource()->name() == name);
        }
    );

    if (it != active_instances_.end() && *it) {
        return std::ref(**it);
    }

    return std::nullopt;
}

std::vector<std::reference_wrapper<SoundInstance>> AudioService::get_all_playing_sounds() const {
    std::vector<std::reference_wrapper<SoundInstance>> instances;

    for (const auto& instance : active_instances_) {
        if (instance) {
            instances.push_back(std::ref(*instance));
        }
    }

    return instances;
}

void AudioService::set_sound_volume(std::unique_ptr<SoundInstance>&& sound_instance, float volume) noexcept {
    if (!sound_instance) {
        return;
    }

    sound_instance->volume(volume);
}

void AudioService::set_sound_volume(const std::string& name, float volume) noexcept {
    auto possible_instance = get_sound_instance(name);

    if (possible_instance) {
        possible_instance->get().volume(volume);
    }
}

void AudioService::update() {
    active_instances_.erase(
        std::remove_if(
            active_instances_.begin(), active_instances_.end(),
            [](const std::unique_ptr<SoundInstance>& instance) {
                return !instance || instance->is_finished();
            }),
        active_instances_.end()
    );
}