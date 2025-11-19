#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

class GameObject;

class Component {
private:
    std::optional<std::reference_wrapper<GameObject>> parent_;
    bool active_ {true};
    bool marked_for_deletion_ {false};

    std::vector<std::function<void(Component&)>> on_detach_actions_;
    std::vector<std::function<void(Component&)>> on_attach_actions_;

public:
    explicit Component();
    virtual ~Component() = default;

    [[nodiscard]] bool active() const noexcept;
    Component& active(bool value) noexcept;

    [[nodiscard]] bool marked_for_deletion() const noexcept;
    Component& mark_for_deletion() noexcept;

    virtual void update(float dt) = 0;
    
    virtual void on_attach();
    virtual void on_detach();

    virtual void on_serialize() = 0;
    virtual void on_deserialize() = 0;

    [[nodiscard]]
    const std::optional<std::reference_wrapper<GameObject>>& parent() const noexcept;
    std::optional<std::reference_wrapper<GameObject>>& parent() noexcept;

    [[nodiscard]] 
    Component& parent(GameObject& parent);
    Component& parent(std::nullopt_t nullopt);

    size_t add_on_attach(const std::function<void(Component&)>& action);
    void remove_on_attach(size_t index);

    size_t add_on_detach(const std::function<void(Component&)>& action);
    void remove_on_detach(size_t index);  
};

template<typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;