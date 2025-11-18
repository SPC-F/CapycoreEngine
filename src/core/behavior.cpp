#include <engine/core/behavior.h>

Behavior::Behavior() : attached_component_(std::nullopt)
{} 

void Behavior::attach(Component& component) 
{
    attached_component_ = component;
}

bool Behavior::enabled() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated Component.");
    }

    return attached_component_->get().active() && game_object().is_active_in_world() && enabled_;
}

Behavior& Behavior::enabled(bool value) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated Component.");
    }

    enabled_ = value;
    return *this;
}


GameObject& Behavior::game_object() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated GameObject.");
    }

    if (!attached_component_->get().parent()) {
        throw std::runtime_error("Component has no associated GameObject.");
    }

    return attached_component_->get().parent()->get();
}

Transform& Behavior::transform() const 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated GameObject.");
    }

    return game_object().transform();
}

void Behavior::destroy() 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated GameObject.");
    }

    // Get scene from parent GameObject and remove the parent GameObject itself
}

void Behavior::destroy(Component& component) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated GameObject.");
    }

    game_object().remove_child(component.parent().value().get());
}

void Behavior::destroy(GameObject& game_object) 
{
    if (!attached_component_) {
        throw std::runtime_error("Behavior has no associated GameObject.");
    }

    // Get scene from parent GameObject and remove the specified game object
}