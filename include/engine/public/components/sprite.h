#pragma once

#include <engine/core/rendering/renderable.h>
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

#include <optional>
#include <string>

/**
 * @brief Component that represents a 2D sprite to be rendered.
 * The Sprite component holds a reference to a Texture and includes properties
 * for flipping, sorting, ordering, and color modulation. It can be treated as a
 * wrapper around a texture with additional rendering properties that go beyond
 * just the texture itself.
 *
 *
 * Usage:
 * - Create a Sprite component with a texture name and desired properties.
 * - Attach the Sprite to a GameObject to have it rendered.
 * - Modify properties such as flip_x, flip_y, sorting_layer, ordering_layer,
 *   and color as needed.
 * @note Ensure the specified texture exists in the asset resource.
 */
class Sprite final : public Renderable {
 private:
  std::reference_wrapper<Texture> texture_;
  int flip_x_;
  int flip_y_;
  int sorting_layer_;
  int ordering_layer_;
  Color color_;

  std::reference_wrapper<Texture> get_texture_for(const std::string& sprite);

 public:
  Sprite(const std::string& sprite, Color color, bool flip_x, bool flip_y,
         int sorting_layer, int ordering_layer);

  [[nodiscard]] bool flip_x() const;
  Sprite& flip_x(bool val);

  [[nodiscard]] bool flip_y() const;
  Sprite& flip_y(bool val);

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

  std::string type_name() const override;
};
