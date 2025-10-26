#include <engine/public/component.h>
#include <engine/public/gameObject.h>

Component::Component(GameObject& parent)
    : parent_(std::ref(parent)), active_(true) {}

bool Component::active() const noexcept {
    return active_;
}

Component& Component::active(const bool value) noexcept {
    active_ = value;
    return *this;
}