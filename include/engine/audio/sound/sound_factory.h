#pragma once

#include <memory>
#include <string>

#include <engine/audio/sound/sound_type.h>
#include <engine/audio/sound/sound_resource.h>
#include <engine/audio/sound/sound_instance.h>

/**
 * @brief Factory class for creating SoundResource instances based on SoundType.
 * The reason for shared pointers is to manage the lifetime of sound resources and instances
 * easily share them across different parts of the audio system without worrying about ownership.
 */
class SoundFactory {
public:
    SoundFactory() = default;
    ~SoundFactory() = default;

    static std::shared_ptr<SoundResource> create_sound_resource(const std::string& file_path, const std::string& name, SoundType type = SoundType::GENERIC);
    static std::shared_ptr<SoundInstance> create_sound_instance(std::shared_ptr<SoundResource> resource, float volume = 1.0f);
};  
