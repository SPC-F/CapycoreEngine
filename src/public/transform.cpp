#include <engine/public/transform.h>

Transform::Transform()
    : Transform(Vector3(0.0f, 0.0f, 0.0f), 0.0f, 1) {}

Transform::Transform(const Vector3 position)
    : Transform(position, 0.0f, 1) {}

Transform::Transform(const Vector3 position, const float rotation, const int scale)
    : Transform(position, rotation, scale, std::nullopt) {}

Transform::Transform(const Vector3 position, const float rotation, const int scale, std::optional<std::reference_wrapper<Transform>> parent)
    : position_(position), rotation_(rotation), scale_(scale), parent_(parent){}

Transform& Transform::relativePosition(const Vector3& pos) noexcept {
    position_ = pos;
    return *this;
}

Vector3 Transform::relativePosition() const noexcept {
    return position_;
}

Vector3 Transform::absolutePosition() const noexcept {
    if (!parent_.has_value()) {
        return position_;
    }
    const auto& parentPos = parent_.value().get().absolutePosition();
    return position_ + parentPos;
}

Transform& Transform::rotation(const float rot) noexcept {
    rotation_ = rot;
    return *this;
}

float Transform::rotation() const noexcept {
    return rotation_;
}

Transform& Transform::scale(const int scale) noexcept {
    scale_ = scale;
    return *this;
}

int Transform::scale() const noexcept {
    return scale_;
}

Transform& Transform::parent(const std::optional<std::reference_wrapper<Transform>> parent) noexcept {
    parent_ = parent;
    return *this;
}

std::optional<std::reference_wrapper<Transform>> Transform::parent() const noexcept {
    if (parent_) {
        return parent_->get();
    }
    return std::nullopt;
}