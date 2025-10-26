#pragma once
#include <algorithm>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include <engine/public/scene.h>
#include <engine/public/transform.h>
#include <engine/public/component.h>

class GameObject {
private:
    std::vector<std::unique_ptr<Component>> components_ {};
    std::vector<std::reference_wrapper<GameObject>> children_ {};

    std::optional<std::reference_wrapper<GameObject>> parent_;
    bool is_active_ {true};
    bool is_active_in_world_ {true};

    std::string name_;
    std::string tag_;
    int layer_ {};
    Scene& scene_;
    Transform transform_;

public:
    explicit GameObject(Scene& scene);
    virtual ~GameObject();

    GameObject& parent(GameObject& parent);
    GameObject& parent(std::nullopt_t);

    [[nodiscard]] bool is_active_in_world() const noexcept;
    [[nodiscard]] bool is_active() const noexcept;

    void set_inactive() noexcept;
    void set_active() noexcept;
    void set_active_in_world() noexcept;
    void set_inactive_in_world() noexcept;

    GameObject& name(const std::string& name);
    [[nodiscard]] const std::string& name() const;

    GameObject& tag(const std::string& tag);
    [[nodiscard]] const std::string& tag() const;

    GameObject& layer(int layer);
    [[nodiscard]] int layer() const;

    GameObject& transform(Transform transform);
    [[nodiscard]] Transform& transform();

    [[nodiscard]] const Scene& scene() const noexcept;

    std::vector<std::reference_wrapper<GameObject>>& children();
    GameObject& add_child(GameObject& child);
    GameObject& remove_child(GameObject& child);

    template<IsComponent T>
    std::optional<std::reference_wrapper<T>> get_component() const noexcept {
        for (auto& component : components_) {
            if (auto& casted = dynamic_cast<T*>(component.get())) {
                return std::ref(*casted);
            }
        }
        return std::nullopt;
    }

    /** Get all components of type T attached to this GameObject.
        Note that it does NOT include the components attached to the
        child objects. Those can be retrieved with GameObject::getComponentsInChildren */
    template<IsComponent T>
    std::vector<std::reference_wrapper<T>> get_components() const {
        auto filtered = components_
            | std::views::filter([](auto& c) { return dynamic_cast<T*>(c.get()); })
            | std::views::transform([](auto& c) -> std::reference_wrapper<T> {
                return std::ref(*static_cast<T*>(c.get()));
            });

        return std::vector<std::reference_wrapper<T>>(filtered.begin(), filtered.end());
    }

    template<IsComponent T>
    std::vector<std::reference_wrapper<T>> get_components_from_children() const {
        std::vector<std::reference_wrapper<T>> result {};

        for (auto& child : children_) {
            auto childComponents = child.get().get_components_from_children<T>();
            result.insert(result.end(), childComponents.begin(), childComponents.end());
        }

        return result;
    }

    template<IsComponent T, typename... Args>
    T& add_component(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *component;
        components_.emplace_back(std::move(component));
        return ref;
    }

    template<IsComponent T>
    void remove_component(T& component) {
        components_.erase(
            std::remove_if(
                components_.begin(),
                components_.end(),
                [&component](const std::unique_ptr<Component>& c) {
                    return c.get() == &component;
                }
            ),
            components_.end()
        );
    }

    void serialize() const;
    void deserialize() const;
};
