#include <engine/public/gameObject.h>
#include <engine/public/component.h>

GameObject::GameObject(Scene& scene) : scene(scene) {}
GameObject::~GameObject() {
    if (parent_.has_value()) {
        parent_->get().removeChild(*this);
    }
}

void GameObject::setInactive() noexcept {
    isActive_ = false;
}
void GameObject::setActive() noexcept {
    isActive_ = true;
}
void GameObject::setActiveInWorld() noexcept {
    isActiveInWorld_ = true;
}
void GameObject::setInactiveInWorld() noexcept {
    isActiveInWorld_ = false;
}

bool GameObject::isActiveInWorld() const noexcept {
    return isActiveInWorld_;
}

bool GameObject::isActive() const noexcept {
    return isActive_;
}

GameObject& GameObject::parent(GameObject& parent) {
    parent_ = parent;
    return *this;
}

GameObject& GameObject::parent(std::nullopt_t) {
    if (!parent_.has_value()) {
        return *this;
    }

    parent_ = std::nullopt;
    return *this;
}

std::vector<std::reference_wrapper<GameObject>>& GameObject::children() {
    return children_;
}

GameObject& GameObject::addChild(GameObject& child) {
    children_.emplace_back(child);
    return *this;
}

GameObject& GameObject::removeChild(GameObject& child) {
    std::erase_if(children_, [&](auto& ref) {
        return &ref.get() == &child;
    });
    child.parent(std::nullopt);
    return *this;
}

void GameObject::serialize() const {
    throw std::runtime_error("Not implemented");
}
void GameObject::deserialize() const {
    throw std::runtime_error("Not implemented");
}