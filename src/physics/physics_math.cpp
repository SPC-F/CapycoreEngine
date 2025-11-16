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
};
