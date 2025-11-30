#pragma once

#include <string>

#include <engine/public/ui/ui_object.h>
#include <engine/public/components/ui/image.h>

class UIImage : public UIObject {
public:
    UIImage(
        Scene& scene,
        std::string image_path,
        float width,
        float height,
        Point pivot,
        Point anchor
    );

    void update(float dt) override;

    [[nodiscard]] const std::string& image_path() const;
    UIImage& image_path(const std::string& path);

    [[nodiscard]] Color color() const;
    UIImage& color(Color color);

private:
    std::string image_path_;
    Color color_;
};
