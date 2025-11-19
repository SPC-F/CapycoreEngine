#include <engine/public/behavior.h>

Behavior::Behavior() : attached_component_(std::nullopt)
{} 

void Behavior::attach(Component& component) 
{
    attached_component_ = component;
}

bool Behavior::enabled() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    return attached_component_->get().active() && game_object().is_active_in_world() && enabled_;
}

Behavior& Behavior::enabled(bool value) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    enabled_ = value;
    return *this;
}


GameObject& Behavior::game_object() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    if (auto parent = attached_component_->get().parent()) {
        return parent->get();
    }

    throw std::runtime_error("Component has no associated GameObject.");
}

Transform& Behavior::transform() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    return game_object().transform();
}

void Behavior::destroy() 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    game_object().mark_for_deletion();
}

void Behavior::destroy(Component& component) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    if (auto parent = component.parent()) {
        game_object().remove_child(parent->get());
    }
}

void Behavior::destroy(GameObject& game_object) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated component.");
    }

    // Get scene from parent GameObject and remove the specified game object
}