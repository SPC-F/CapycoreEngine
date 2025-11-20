#pragma once
#include <memory>
#include <optional>

class GameObject;

class Component {
private:
    std::optional<std::reference_wrapper<GameObject>> parent_;
    bool active_ {true};
protected:
    [[nodiscard]] std::optional<std::reference_wrapper<GameObject>> parent() const noexcept;
public:
    explicit Component();
    virtual ~Component() = default;

    [[nodiscard]] bool active() const noexcept;
    Component& active(bool value) noexcept;

    virtual void update(float dt) = 0;
    virtual void on_attach() = 0;
    virtual void on_detach() = 0;
    virtual void on_serialize() = 0;
    virtual void on_deserialize() = 0;

    Component& parent(GameObject& parent);
    Component& parent(std::nullopt_t nullopt);
};

template<typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;