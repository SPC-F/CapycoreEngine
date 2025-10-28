#include <engine/audio/audio_manager.h>

SoundInstance* AudioManager::register_sound(const std::string& file_path, const std::string& name) {
    // 
}
bool AudioManager::unregister_sound(const std::string& name) {
    // 
}

SoundInstance* AudioManager::get_sound(const std::string& name) noexcept {
    // 
}
std::vector<SoundInstance*> AudioManager::get_all_sounds() {
    //
}
std::vector<SoundInstance*> AudioManager::get_all_playing_sounds() noexcept {
    //
}

void AudioManager::play_sound(const SoundInstance& sound_instance) {
    //
}
void AudioManager::play_sound(const std::string& name) {
    play_sound(*get_sound(name));
}

void AudioManager::stop_sound(const SoundInstance& sound_instance) {
    //
}
void AudioManager::stop_sound(const std::string& name) {
    stop_sound(*get_sound(name));
}
void AudioManager::stop_all_sounds() {
    //
}

void AudioManager::set_sound_volume(SoundInstance& sound_instance, float volume) noexcept {
    //
}
void AudioManager::set_sound_volume(const std::string& name, float volume) noexcept {
    set_sound_volume(*get_sound(name), volume);
}

void AudioManager::update() {
    //
}