#include <engine/audio/sound/resource/sound_resource.h>

SoundResource::SoundResource(std::string name, std::string file_path, SoundType type)
    : name_(std::move(name)), file_path_(std::move(file_path)), type_(type) {}

const SoundType& SoundResource::type() const noexcept {
    return type_;
}

const std::string& SoundResource::name() const noexcept {
    return name_;
}

const std::string& SoundResource::file_path() const noexcept {
    return file_path_;
}