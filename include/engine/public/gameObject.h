#pragma once
#include <algorithm>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "component.h"
#include "scene.h"
#include "transform.h"

class GameObject {
private:
    /** GameObject owns its components.
    Alternative: Make it shared so multiple objects can share components.
    It would be nice if you want to do complex shared behaviors but its complex, don't. */
    std::vector<std::unique_ptr<Component>> components_ {};

    /** GameObject does not own its children. Why? Because every GameObject instance is owned by a scene.
        If you turn this around, a Scene would only own root objects. That works, but it isn't what we
        want to happen. A shared_ptr would also not work because you would get weird undefined behaviors.
        In this case its paramount that a GameObject is destroyed with the intention to keep its
        children alive, or to have them be destroyed all together on destruction. */
    std::vector<std::weak_ptr<GameObject>> children_ {};

    std::optional<std::reference_wrapper<GameObject>> parent_;
    bool isActive_ {true};

public:
    // TODO: Could we for the love of god make some of these immutable and private? I do not know if these have to be public or mutable right now...
    std::string name;
    std::string tag;
    int layer {};
    Scene& scene;
    Transform transform;

    explicit GameObject(const Scene& scene);
    virtual ~GameObject();

    [[nodiscard]] bool isActiveInWorld() const noexcept;
    [[nodiscard]] bool isActive() const noexcept;

    GameObject& parent(GameObject& parent);
    GameObject& parent(std::nullopt_t);

    std::vector<std::weak_ptr<GameObject>>& children();
    GameObject& addChild(const std::shared_ptr<GameObject>& child);
    GameObject& removeChild(GameObject& child);

    template<IsComponent T>
    std::optional<std::reference_wrapper<T>> getComponent() const noexcept {
        for (auto& component : components_) {
            if (auto& casted = dynamic_cast<T*>(component.get())) {
                return std::ref(casted);
            }
        }
        return std::nullopt;
    }

    /** Get all components of type T attached to this GameObject.
        Note that it does NOT include the components attached to the
        child objects. Those can be retrieved with GameObject::getComponentsInChildren */
    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getComponents() const {
        auto filtered = components_
            | std::views::filter([](auto& c) { return dynamic_cast<T*>(c.get()); })
            | std::views::transform([](auto& c) -> std::reference_wrapper<T> {
                return std::ref(*static_cast<T*>(c.get()));
            });

        return std::vector<std::weak_ptr<T>>(filtered.begin(), filtered.end());
    }

    template<IsComponent T>
    std::vector<std::reference_wrapper<T>> getComponentsFromChildren() const {
        std::vector<std::reference_wrapper<T>> result {};

        for (auto& childWeak : children_) {
            if (const auto child = childWeak.lock()) {
                auto childComponents = child->getComponentsFromChildren<T>();
                result.insert(result.end(), childComponents.begin(), childComponents.end());
            }
        }

        return result;
    }

    template<IsComponent T, typename... Args>
    T& addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *component;
        components_.emplace_back(std::move(component));
        return ref;
    }

    template<IsComponent T>
    void removeComponent(T& component) {
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
