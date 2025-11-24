#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/util/color.h>

class Camera final : public GameObject {
private:
    Color backgroundColor_{0, 0, 0, 255};
    float aspectWidth_{16.0f};
    float aspectHeight_{9.0f};
    float zoom_{1.0f};
    bool isMain_{false};

public:
    explicit Camera(
        Scene& scene,
        Color backgroundColor,
        float aspectWidth,
        float aspectHeight,
        float zoom,
        bool isMain);

    void set_background_color(const Color& color);
    Color background_color() const;

    void set_aspect_width(float width);
    float aspect_width() const;

    void set_aspect_height(float height);
    float aspect_height() const;

    void set_zoom(float zoom);
    float zoom() const;

    void set_main();
    void set_not_main();
    bool is_main() const;
};