#pragma once

#include <memory>
#include <string>

#include <engine/audio/sound/sound_type.h>

/**
 * @brief Represents a generic sound resource in the audio engine.
 * This class serves as a base for specific sound implementations.
 */
class SoundResource {
public:
    SoundResource(std::string name, std::string file_path, SoundType type = SoundType::GENERIC);
    virtual ~SoundResource() = default;

    [[nodiscard]]
    const SoundType& type() const noexcept;
    [[nodiscard]]
    const std::string& name() const noexcept;
    [[nodiscard]]
    const std::string& file_path() const noexcept;

protected:
    SoundType type_;
    std::string name_;
    std::string file_path_;
};