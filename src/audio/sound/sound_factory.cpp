#include <engine/audio/sound/sound_factory.h>

#include <engine/audio/sound/instance/generic_sound_instance.h>
#include <engine/audio/sound/instance/sdl_sound_instance.h>
#include <engine/audio/sound/resource/sdl_sound_resource.h>

std::shared_ptr<SoundResource> SoundFactory::create_sound_resource(const std::string& file_path, const std::string& name, SoundType type) {
    switch (type) {
        case SoundType::SDL_MIXER:
            return std::make_shared<SDLSoundResource>(name, file_path, type);
        case SoundType::GENERIC:
        default:
            return std::make_shared<SoundResource>(name, file_path, type);
    }
}

std::shared_ptr<SoundInstance> SoundFactory::create_sound_instance(std::shared_ptr<SoundResource> resource, float volume) {
    switch (resource->type())
    {
        case SoundType::SDL_MIXER:
            return std::make_shared<SDLSoundInstance>(std::dynamic_pointer_cast<SDLSoundResource>(resource), volume);
        case SoundType::GENERIC:
        default:
            return std::make_shared<GenericSoundInstance>(resource, volume);
    }
}