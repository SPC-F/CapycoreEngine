#pragma once

#include <optional>
#include <functional>
#include <stdexcept>

#include <engine/public/gameObject.h>
#include <engine/public/component.h>

class Behavior
{
public:
    Behavior();
    virtual ~Behavior() = default;

    void attach(Component& component);

    virtual void on_awake() = 0;
    virtual void on_start() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_destroy() = 0;

    [[nodiscard]] GameObject& game_object() const;
    [[nodiscard]] Transform& transform() const;

    [[nodiscard]] bool enabled() const;
    Behavior& enabled(bool value);

    template<typename T>
    std::optional<std::reference_wrapper<T>> get_component() 
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        return game_object().template get_component<T>();
    }

    template<typename T>
    std::vector<std::reference_wrapper<T>> get_components() 
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        return game_object().template get_components<T>();
    }

    template<typename T>
    std::optional<std::reference_wrapper<T>> get_component_from_children() 
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        for (auto& child : game_object().children()) {
            auto comp = child.get().template get_component<T>();
            if (comp) {
                return comp;
            }
        }

        return std::nullopt;
    }

    template<typename T>
    std::vector<std::reference_wrapper<T>> get_components_from_children() 
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        return game_object().template get_components_from_children<T>();
    }

    template<typename T>
    std::optional<std::reference_wrapper<T>> get_component_in_parent() 
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        auto current_parent = game_object().parent();

        while (current_parent) {
            auto comp = current_parent->get().template get_component<T>();
            if (comp) {
                return comp;
            }
            current_parent = current_parent->get().parent();
        }

        return std::nullopt;
    }

    template<typename T>
    std::vector<std::reference_wrapper<T>> get_components_in_parent()
    {
        if (!attached_component_) {
            throw std::runtime_error("Behavior has no associated GameObject.");
        }

        std::vector<std::reference_wrapper<T>> result;
        auto current_parent = game_object().parent();

        while (current_parent) {
            auto comps = current_parent->get().template get_components<T>();
            result.insert(result.end(), comps.begin(), comps.end());
            
            current_parent = current_parent->get().parent();
        }

        return result;
    }

    void destroy();
    void destroy(Component& component);
    void destroy(GameObject& game_object);

private:
    std::optional<std::reference_wrapper<Component>> attached_component_;
    bool enabled_ {true};
};