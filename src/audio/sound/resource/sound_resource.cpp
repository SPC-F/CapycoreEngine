#include <engine/audio/sound/resource/sound_resource.h>

SoundResource::SoundResource(const std::string& name, const std::string& file_path, SoundType type)
    : name_(name), file_path_(file_path), type_(type) {}

const SoundType& SoundResource::type() const noexcept {
    return type_;
}

const std::string& SoundResource::name() const noexcept {
    return name_;
}

const std::string& SoundResource::file_path() const noexcept {
    return file_path_;
}