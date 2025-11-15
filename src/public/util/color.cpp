#include <engine/public/util/color.h>

constexpr int default_color_value = 255;
Color::Color() : Color(
    default_color_value,
    default_color_value,
    default_color_value,
    default_color_value) {
}

Color::Color(
    const unsigned short r,
    const unsigned short g,
    const unsigned short b,
    const unsigned short a): r(r), g(g), b(b), a(a) {
}