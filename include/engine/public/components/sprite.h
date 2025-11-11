#pragma once
#include <string>
#include <engine/public/util/color.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/component.h>

class Sprite : public Component {
private:
    std::reference_wrapper<Texture> texture_;

    int flipX_;
    int flipY_;
    int sortingLayer_;
    int orderingLayer_;
    Color color_;

public:
    Sprite(GameObject& parent, const std::string& sprite, Color color, int flipX, int flipY, int sortingLayer, int orderingLayer);

    [[nodiscard]] int flip_x() const;
    Sprite& flip_x(int val);

    [[nodiscard]] int flip_y() const;
    Sprite& flip_y(int val);

    [[nodiscard]] int sortingLayer() const;
    Sprite& sortingLayer(int val);

    [[nodiscard]] int orderingLayer() const;
    Sprite& orderingLayer(int val);

    [[nodiscard]] Color color() const;
    Sprite& color(Color color);

    [[nodiscard]] const Texture& texture() const;
    Sprite& texture(const std::string& name);
    Sprite& texture(Texture& texture);
};
