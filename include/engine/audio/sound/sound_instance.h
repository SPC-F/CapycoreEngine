#pragma once

#include <memory>
#include <string>

/**
 * @brief Represents an instance of a generic sound within the audio system.
 */
class SoundInstance {
public:
    SoundInstance(const std::string& name, const std::string& file_path, float volume = 1.0f)
        : name(name), file_path(file_path), volume(volume) {}
    virtual ~SoundInstance() = default;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isPlaying() const = 0;

protected:
    std::string name;
    std::string file_path;

    int loops{0};
    bool playing{false};
    float volume{1.0f};
};