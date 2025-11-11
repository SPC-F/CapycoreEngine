#include <engine/physics/physics_math.h>

namespace PhysicsMath {
    float PhysicsMath::to_meters(float pixels, float ratio) noexcept {
        return pixels / ratio;
    }
    
    float PhysicsMath::to_pixels(float meters, float ratio) noexcept {
        return meters * ratio;
    }

    float PhysicsMath::to_radians(float degrees) noexcept {
        return degrees * (B2_PI / 180.0f); // NOLINT
    }

    float PhysicsMath::to_degrees(float radians) noexcept {
        return radians * (180.0f / B2_PI); // NOLINT
    }
};
