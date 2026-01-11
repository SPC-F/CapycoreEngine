#pragma once

#include <engine/core/rendering/renderable.h>
#include <engine/core/rendering/texture.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>

/**
 * @brief A UI component for displaying images.
 *
 * The Image component allows you to display images in the UI. You can set
 * various properties such as flipping, size, and color tinting.
 *
 * Usage:
 * - Create an Image component with the desired image and properties.
 * - Attach the Image component to a GameObject to display it in the UI.
 * - Modify properties such as flip_x, flip_y, width, height, and color as
 *   needed.
 */
class Image : public Renderable {
 public:
  Image(const std::string& image, int flip_x, int flip_y, int width, int height,
        Color color);  // NOLINT

  void update(float dt) override;

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

  std::string type_name() const override;

 private:
  std::reference_wrapper<Texture> texture_;
  int flip_x_;
  int flip_y_;
  int width_;
  int height_;
  Color color_;

  std::reference_wrapper<Texture> get_texture_for(const std::string& name);
};
