#pragma once

/**
 * @brief Represents a color with red, green, blue, and alpha components.
 * Each component is represented as an unsigned short (0-255).
 */
class Color {
 public:
  unsigned short r;
  unsigned short g;
  unsigned short b;
  unsigned short a;
  Color(unsigned short r, unsigned short g, unsigned short b, unsigned short a);
  Color();
};