#include <engine/audio/audio_service.h>

SoundInstance* AudioService::register_sound(const std::string& file_path, const std::string& name) {
    // 
}
bool AudioService::unregister_sound(const std::string& name) {
    // 
}

SoundInstance* AudioService::get_sound(const std::string& name) const noexcept {
    //
}
std::vector<SoundInstance*> AudioService::get_all_sounds() const noexcept {
    //
}
std::vector<SoundInstance*> AudioService::get_all_playing_sounds() const noexcept {
    //
}

void AudioService::play_sound(const SoundInstance& sound_instance) {
    //
}
void AudioService::play_sound(const std::string& name) {
    play_sound(*get_sound(name));
}

void AudioService::stop_sound(const SoundInstance& sound_instance) {
    //
}
void AudioService::stop_sound(const std::string& name) {
    stop_sound(*get_sound(name));
}
void AudioService::stop_all_sounds() {
    //
}

void AudioService::set_sound_volume(SoundInstance& sound_instance, float volume) noexcept {
    //
}
void AudioService::set_sound_volume(const std::string& name, float volume) noexcept {
    set_sound_volume(*get_sound(name), volume);
}

void AudioService::update() {
    //
}