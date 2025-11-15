#pragma once

#include <memory>
#include <string>

#include <engine/audio/sound/instance/sound_instance.h>
#include <engine/audio/sound/resource/sound_resource.h>
#include <engine/audio/sound/sound_type.h>

/**
 * @brief Factory class for creating SoundResource instances based on SoundType.
 * The reason for shared pointers is to manage the lifetime of sound resources and instances
 * easily share them across different parts of the audio system without worrying about ownership.
 */
class SoundFactory {
public:
    SoundFactory() = delete;
    ~SoundFactory() = delete;

    static std::shared_ptr<SoundResource> create_sound_resource(const std::string& file_path, const std::string& name, SoundType type = SoundType::GENERIC);
    static std::unique_ptr<SoundInstance> create_sound_instance(std::shared_ptr<SoundResource>& resource, float volume = 1.0f);
};  
