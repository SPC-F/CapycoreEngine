#pragma once
#include <memory>
#include <optional>

#include <engine/public/util/vector3.h>

class Transform {
private:
    Vector3 local_position_;
    float rotation_{};
    Vector3 scale_{};
    std::optional<std::reference_wrapper<Transform>> parent_;
public:

    Transform();
    explicit Transform(Vector3 position);
    Transform(Vector3 position, float rotation, Vector3 scale);
    Transform(Vector3 position, float rotation, Vector3 scale, std::optional<std::reference_wrapper<Transform>> parent);

    [[nodiscard]] Vector3 local_position() const noexcept;

    [[nodiscard]] Vector3 position() const noexcept;
    Transform& position(const Vector3& pos) noexcept;

    Transform& rotation(float rot) noexcept;
    [[nodiscard]] float rotation() const noexcept;

    Transform& scale(Vector3 scale) noexcept;
    [[nodiscard]] Vector3 scale() const noexcept;

    Transform& parent(std::optional<std::reference_wrapper<Transform>> parent) noexcept;
    [[nodiscard]] std::optional<std::reference_wrapper<Transform>> parent() const noexcept;
};
