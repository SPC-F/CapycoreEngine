#include <engine/public/gameObject.h>
#include <engine/public/component.h>

GameObject::GameObject(Scene& scene) :
        id_ {boost::uuids::random_generator()()},
        scene_(scene) {

}

GameObject::~GameObject() {
    if (parent_.has_value()) {
        parent_->get().remove_child(*this);
    }
}

boost::uuids::uuid GameObject::id() const noexcept {
    return id_;
}

GameObject& GameObject::name(const std::string& name) {
    name_ = name;
    return *this;
}
const std::string& GameObject::name() const {
    return name_;
}

GameObject& GameObject::tag(const std::string& tag) {
    tag_ = tag;
    return *this;
}
const std::string& GameObject::tag() const {
    return tag_;
}

GameObject& GameObject::layer(const int layer) {
    layer_ = layer;
    return *this;
}
int GameObject::layer() const {
    return layer_;
}

GameObject& GameObject::transform(Transform transform) {
    transform_ = transform;
    return *this;
}
Transform& GameObject::transform() {
    return transform_;
}

const Scene& GameObject::scene() const noexcept {
    return scene_;
}

void GameObject::set_inactive() noexcept {
    is_active_ = false;
}
void GameObject::set_active() noexcept {
    is_active_ = true;
}
void GameObject::set_active_in_world() noexcept {
    is_active_in_world_ = true;
}
void GameObject::set_inactive_in_world() noexcept {
    is_active_in_world_ = false;
}

bool GameObject::is_active_in_world() const noexcept {
    return is_active_in_world_;
}

bool GameObject::is_active() const noexcept {
    return is_active_;
}

GameObject& GameObject::parent(GameObject& parent) {
    parent_ = parent;
    return *this;
}

GameObject& GameObject::parent(std::nullopt_t null_opt) {
    if (!parent_.has_value()) {
        return *this;
    }

    parent_ = std::nullopt;
    return *this;
}

std::vector<std::reference_wrapper<GameObject>>& GameObject::children() {
    return children_;
}

GameObject& GameObject::add_child(GameObject& child) {
    children_.emplace_back(child);
    return *this;
}

GameObject& GameObject::remove_child(GameObject& child) {
    std::erase_if(children_, [&](auto& ref) {
        return &ref.get() == &child;
    });
    child.parent(std::nullopt);
    return *this;
}

// NOLINTBEGIN
void GameObject::serialize() const {
    throw std::runtime_error("Not implemented");
}
void GameObject::deserialize() const {
    throw std::runtime_error("Not implemented");
}
// NOLINTEND