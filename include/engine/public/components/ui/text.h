#pragma once

#include <engine/core/rendering/renderable.h>
#include <engine/public/component.h>
#include <engine/public/util/color.h>
#include <engine/public/util/point.h>

#include <string>

/**
 * @brief Text alignment options
 */
enum TextAlignment : uint8_t {
  Left = 0,
  Center = 1,
  Right = 2,
};

/**
 * @brief UI Text Component
 *
 * Represents a text element in the UI with customizable font, size, color, and
 * content. Inherits from Renderable to allow rendering strategies.
 *
 * Usage:
 * - Create a Text component with desired properties.
 * - Attach the Text component to a GameObject to display text in the UI.
 * - Modify properties such as text content, font, size, color, alignment, and
 *   offset as needed.
 * @note Ensure the specified font is available in the assets.
 */
class Text : public Renderable {
 public:
  Text(std::string text, std::string font, std::string font_path, int font_size,
       Color color);

  void update(float dt) override {}

  [[nodiscard]] const std::string& text() const;
  Text& text(const std::string& text);

  [[nodiscard]] const std::string& font() const;
  Text& font(const std::string& font);

  [[nodiscard]] const std::string& font_path() const;
  Text& font_path(const std::string& font_path);

  [[nodiscard]] int font_size() const;
  Text& font_size(int font_size);

  [[nodiscard]] Color color() const;
  Text& color(Color color);

  [[nodiscard]] bool dirty() const;
  void mark_dirty(bool dirty = true);

  std::string type_name() const override;

  [[nodiscard]] TextAlignment alignment() const;
  Text& alignment(TextAlignment alignment);

  [[nodiscard]] Point offset() const;
  Text& offset(Point offset);

 private:
  std::string text_;
  std::string font_;
  std::string font_path_;

  int font_size_;
  Color color_;

  TextAlignment alignment_{Center};
  Point offset_{0, 0};

  bool dirty_;
};