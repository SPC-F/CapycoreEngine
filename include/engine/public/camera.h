#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/util/color.h>

class Camera final : public GameObject {
private:
    Color backgroundColor_;
    float zoom_{1.0f};
    bool isMain_{false};

public:
    explicit Camera(
        Scene& scene,
        Color backgroundColor,
        float zoom,
        bool isMain);

    void set_background_color(const Color& color);
    [[nodiscard]] Color background_color() const;

    void set_zoom(float zoom);
    [[nodiscard]] float zoom() const;

    void set_main();
    void set_not_main();
    [[nodiscard]] bool is_main() const;
};