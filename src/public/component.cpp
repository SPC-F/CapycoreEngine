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

bool Component::marked_for_deletion() const noexcept {
    return marked_for_deletion_;
}

Component& Component::mark_for_deletion() noexcept {
    marked_for_deletion_ = true;
}

Component& Component::parent(GameObject& parent) {
    parent_ = std::ref(parent);
    return *this;
}

Component& Component::parent(std::nullopt_t nullopt) {
    parent_ = std::nullopt;
    return *this;
}