#include <engine/audio/audio_service.h>

#include <ranges>
#include <algorithm>

#include <engine/audio/sound/sound_factory.h>

std::optional<std::shared_ptr<SoundResource>> AudioService::register_sound(const std::string& file_path, const std::string& name, SoundType type) {
    auto it = sound_resources_.find(name);

    if (it != sound_resources_.end()) {
        return it->second;
    }

    try {
        auto resource = SoundFactory::create_sound_resource(file_path, name, type);
        sound_resources_[name] = resource;

        return resource;
    } catch (...) {
        return std::nullopt;
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

std::optional<std::shared_ptr<SoundResource>> AudioService::get_sound_resource(const std::string& name) const noexcept {
    auto it = sound_resources_.find(name);

    if (it != sound_resources_.end()) {
        return it->second;
    }

    return std::nullopt;
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
    auto sound_resource = get_sound_resource(name);

    if (!sound_resource) {
        throw std::invalid_argument("Sound resource not found: " + name);
    }

    return play_sound(*sound_resource, volume, loop);
}

void AudioService::stop_sound(std::unique_ptr<SoundInstance>&& sound_instance) {
    if (!sound_instance) {
        return;
    }

    auto instance = std::move(sound_instance);

    instance->stop();

    active_instances_.erase(
        std::remove_if(
            active_instances_.begin(), active_instances_.end(),
            [&instance](const std::unique_ptr<SoundInstance>& inst) {
                return inst.get() == instance.get();
            }),
        active_instances_.end()
    );

    instance.reset();
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