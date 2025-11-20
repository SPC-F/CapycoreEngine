#include <engine/public/component.h>
#include <engine/public/gameObject.h>

Component::Component()
    : parent_(std::nullopt) {}

bool Component::active() const noexcept {
    return active_;
}

Component& Component::active(const bool value) noexcept {
    active_ = value;
    return *this;
}

Component& Component::parent(GameObject& parent) {
    parent_ = std::ref(parent);
    return *this;
}

Component& Component::parent(std::nullopt_t nullopt) {
    parent_ = std::nullopt;
    return *this;
}

std::optional<std::reference_wrapper<GameObject>> Component::parent() const noexcept {
    return parent_;
}
