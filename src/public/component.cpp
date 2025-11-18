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

void Component::on_attach() {
    for (auto& action : on_attach_actions_) {
        action(*this);
    }
}

void Component::on_detach() {
    for (auto& action : on_detach_actions_) {
        action(*this);
    }
}

std::optional<std::reference_wrapper<GameObject>>& Component::parent() noexcept {
    return parent_;
}

const std::optional<std::reference_wrapper<GameObject>>& Component::parent() const noexcept {
    return parent_;
}

Component& Component::parent(GameObject& parent) {
    parent_ = std::ref(parent);
    return *this;
}

Component& Component::parent(std::nullopt_t nullopt) {
    parent_ = std::nullopt;
    return *this;
}

size_t Component::add_on_attach(const std::function<void(Component&)>& action) {
    on_attach_actions_.push_back(action);
    return on_attach_actions_.size() - 1;
}

void Component::remove_on_attach(size_t index) {
    if (index < on_attach_actions_.size()) {
        on_attach_actions_.erase(std::next(on_attach_actions_.begin(), static_cast<std::ptrdiff_t>(index)));
    }
}

size_t Component::add_on_detach(const std::function<void(Component&)>& action) {
    on_detach_actions_.push_back(action);
    return on_detach_actions_.size() - 1;
}

void Component::remove_on_detach(size_t index) {
    if (index < on_detach_actions_.size()) {
        on_detach_actions_.erase(std::next(on_detach_actions_.begin(), static_cast<std::ptrdiff_t>(index)));
    }
}