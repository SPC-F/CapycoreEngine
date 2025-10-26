#pragma once
#include <memory>
#include <optional>

#include <engine/public/util/vector3.h>

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

    Transform& relative_position(const Vector3& pos) noexcept;
    [[nodiscard]] Vector3 relative_position() const noexcept;
    [[nodiscard]] Vector3 absolute_position() const noexcept;

    Transform& rotation(float rot) noexcept;
    [[nodiscard]] float rotation() const noexcept;

    Transform& scale(int scale) noexcept;
    [[nodiscard]] int scale() const noexcept;

    Transform& parent(std::optional<std::reference_wrapper<Transform>> parent) noexcept;
    [[nodiscard]] std::optional<std::reference_wrapper<Transform>> parent() const noexcept;
};
