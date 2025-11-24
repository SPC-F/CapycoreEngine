#pragma once

#include <vector>

#include <engine/public/gameObject.h>
#include <engine/public/component.h>
#include <engine/core/rendering/texture.h>

class Animator final : public Component {
private:
    const std::vector<std::reference_wrapper<Texture>> frames_;
    const int interval_ms_;

    int current_texture_index_;
    int accumulator_time_ms_;

    bool is_playing_;
    bool is_looping_;

    int calculate_next_frame_index(int intervals_advanced) const;
    void update_sprite_texture(int new_frame_index);
public:
    explicit Animator(const std::string& sprite_sheet_name, int interval_ms);

    void play(bool is_looping);
    void pause();
    void reset();
    bool is_playing() const noexcept;

    // Component overrides
    void update(float dt_seconds) override;
    void on_serialize() override;
    void on_deserialize() override;
};