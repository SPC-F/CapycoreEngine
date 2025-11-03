#pragma once

#include <memory>
#include <string>

/**
 * @brief Represents a generic sound resource in the audio engine.
 * This class serves as a base for specific sound implementations.
 */
class SoundResource {
public:
    SoundResource(const std::string& name, const std::string& file_path, float volume = 1.0f)
        : name(name), file_path(file_path), volume(volume) {}
    virtual ~SoundResource() = default;

protected:
    std::string name;
    std::string file_path;

    int loops{0};
    bool playing{false};
    float volume{1.0f};
};