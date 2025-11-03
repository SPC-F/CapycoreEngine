#include <engine/audio/audio_service.h>

SoundResource* AudioService::register_sound(const std::string& file_path, const std::string& name) {
    // 
}

bool AudioService::unregister_sound(const std::string& name) {
    // 
}

SoundResource* AudioService::get_sound_resource(const std::string& name) const noexcept {
    //
}
SoundInstance* AudioService::get_sound_instance(const std::string& name) const noexcept {
    //
}
std::vector<SoundInstance*> AudioService::get_all_playing_sounds() const noexcept {
    //
}

void AudioService::play_sound(const SoundResource& sound_resource) {
    //
}
void AudioService::play_sound(const std::string& name) {
    play_sound(*get_sound_resource(name));
}

void AudioService::stop_sound(const SoundInstance& sound_instance) {
    //
}
void AudioService::stop_sound(const std::string& name) {
    stop_sound(*get_sound_instance(name));
}
void AudioService::stop_all_sounds() {
    //
}

void AudioService::set_sound_resource_volume(SoundResource& sound_resource, float volume) noexcept {
    //
}
void AudioService::set_sound_instance_volume(SoundInstance& sound_instance, float volume) noexcept {
    //
}
void AudioService::set_sound_volume(const std::string& name, float volume) noexcept {
    // if sound resource exists, set its volume
    // if sound instance exists, set its volume
}

void AudioService::update() {
    //
}