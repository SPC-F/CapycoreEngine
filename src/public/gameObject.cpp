#include <engine/public/gameObject.h>
#include <engine/public/component.h>

GameObject::GameObject(Scene& scene) : scene(scene) {}
GameObject::~GameObject() {
    if (parent_.has_value()) {
        parent_->get().removeChild(*this);
    }
}


// TODO: Discuss whether this should be recursive (i.e., check parents' active states)
// TODO: Discuss if a GameObject its active state should be determined by its parent
bool GameObject::isActiveInWorld() const noexcept {
    if (parent_.has_value()) {
        return parent_->get().isActiveInWorld() && isActive_;
    }
    return isActive_;
}

// TODO: Discuss whether this should be recursive (i.e., check parents' active states)
// TODO: Discuss if a GameObject its active state should be determined by its parent
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

std::vector<std::weak_ptr<GameObject>>& GameObject::children() {
    return children_;
}

GameObject& GameObject::addChild(const std::shared_ptr<GameObject>& child) {
    children_.push_back(child);
    child->parent(*this);
    return *this;
}

GameObject& GameObject::removeChild(GameObject& child) {
    std::erase_if(
        children_,
        [&child](const std::weak_ptr<GameObject>& ptr) {
            if (const auto locked = ptr.lock()) {
                return locked.get() == &child;
            }
            return false;
        }
    );
    child.parent(std::nullopt);
    return *this;
}

void GameObject::serialize() const {
    throw std::runtime_error("Not implemented");
}
void GameObject::deserialize() const {
    throw std::runtime_error("Not implemented");
}