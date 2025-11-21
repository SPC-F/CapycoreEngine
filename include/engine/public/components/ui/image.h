#pragma once

#include <engine/core/rendering/texture.h>
#include <engine/public/component.h>
#include <engine/core/rendering/renderable.h>
#include <engine/public/util/color.h>

class Image : public Renderable {
public:
    Image(
        const std::string image,
        int flip_x,
        int flip_y,
        int width,
        int height,
        Color color
    );

    void update(float dt) override;
    void on_serialize() override;
    void on_deserialize() override;

    [[nodiscard]] int flip_x() const;
    Image& flip_x(int val);

    [[nodiscard]] int flip_y() const;
    Image& flip_y(int val);

    [[nodiscard]] int width() const;
    Image& width(int val);

    [[nodiscard]] int height() const;
    Image& height(int val);

    [[nodiscard]] Color color() const;
    Image& color(Color color);

    [[nodiscard]] const Texture& texture() const;
    Image& texture(const std::string& name);
    Image& texture(Texture& texture);

private:
    std::reference_wrapper<Texture> texture_;
    int flip_x_;
    int flip_y_;
    int width_;
    int height_;
    Color color_;

    std::reference_wrapper<Texture> get_texture_for(const std::string& name);
};

