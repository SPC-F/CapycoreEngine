#include <engine/physics/physics_math.h>

namespace PhysicsMath {
    float to_meters(float pixels, float ratio) noexcept {
        return pixels / ratio;
    }
    
    float to_pixels(float meters, float ratio) noexcept {
        return meters * ratio;
    }

    float to_radians(float degrees) noexcept {
        return degrees * (B2_PI / 180.0f); // NOLINT
    }

    float to_degrees(float radians) noexcept {
        return radians * (180.0f / B2_PI); // NOLINT
    }

    float convert_box2d_angle_to_radians(float sine, float cosine) noexcept {
        return std::atan2(sine, cosine);
    }

    float convert_radians_to_box2d_sine(float radians) noexcept {
        return std::sin(radians);
    }

    float convert_radians_to_box2d_cosine(float radians) noexcept {
        return std::cos(radians);
    }

    float distance_between_points(const Point& a, const Point& b) noexcept {
        float dx = dx_between_points(a, b);
        float dy = dy_between_points(a, b);
        return std::sqrt((dx * dx) + (dy * dy));
    }

    float dx_between_points(const Point& a, const Point& b) noexcept {
        return b.x - a.x;
    }

    float dy_between_points(const Point& a, const Point& b) noexcept {
        return b.y - a.y;
    }
};
