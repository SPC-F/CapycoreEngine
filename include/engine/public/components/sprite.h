#pragma once
#include <string>
#include <engine/public/util/color.h>
#include <../../core/rendering/renderables/texture.h>
#include <engine/public/component.h>

/**
 * @brief Component that represents a 2D sprite to be rendered.
 * The Sprite component holds a reference to a Texture and includes properties for flipping, sorting, ordering, and color modulation.
 * It can be treated as a wrapper around a texture with additional rendering properties that go beyond just the texture itself.
 */
class Sprite : public Component {
private:
    std::reference_wrapper<Texture> texture_;

    int flip_x_;
    int flip_y_;
    int sorting_layer_;
    int ordering_layer_;
    Color color_;

public:
    Sprite(const std::string& sprite, Color color, int flip_x, int flip_y, int sorting_layer, int ordering_layer);

    [[nodiscard]] int flip_x() const;
    Sprite& flip_x(int val);

    [[nodiscard]] int flip_y() const;
    Sprite& flip_y(int val);

    [[nodiscard]] int sorting_layer() const;
    Sprite& sorting_layer(int val);

    [[nodiscard]] int ordering_layer() const;
    Sprite& ordering_layer(int val);

    [[nodiscard]] Color color() const;
    Sprite& color(Color color);

    [[nodiscard]] const Texture& texture() const;
    Sprite& texture(const std::string& name);
    Sprite& texture(Texture& texture);

    void update(float dt) override;
    void on_attach() override;
    void on_detach() override;
    void on_serialize() override;
    void on_deserialize() override;
};
