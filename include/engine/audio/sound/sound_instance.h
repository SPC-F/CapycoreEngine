#pragma once

#include <memory>
#include <string>

/**
 * @brief Represents an instance of a generic sound within the audio system.
 */
struct SoundInstance {
    SoundInstance() = default;
    virtual ~SoundInstance() = default;

    std::string name;
    std::string file_path;

    int loops{0};
    bool playing{false};
    float volume{1.0f};
};