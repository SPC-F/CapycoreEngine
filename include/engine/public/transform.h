#pragma once
#include <memory>
#include <optional>

#include "util/vector3.h"

class Transform {
private:
    Vector3 position_;
    float rotation_{};
    int scale_{};
    std::optional<std::reference_wrapper<Transform>> parent_;
public:

    Transform();
    explicit Transform(Vector3 position);
    Transform(Vector3 position, float rotation, int scale);
    Transform(Vector3 position, float rotation, int scale, std::optional<std::reference_wrapper<Transform>> parent);

    Transform& position(const Vector3& pos) noexcept;
    [[nodiscard]] Vector3 position() const noexcept;

    Transform& rotation(float rot) noexcept;
    [[nodiscard]] float rotation() const noexcept;

    Transform& scale(int scale) noexcept;
    [[nodiscard]] int scale() const noexcept;

    Transform& parent(Transform& parent) noexcept;
    [[nodiscard]] std::optional<std::reference_wrapper<Transform>> parent() const noexcept;
};
