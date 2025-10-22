#pragma once
#include <memory>

#include <engine/public/gameObject.h>

/**
 * @brief Base class for all components that can be attached to GameObjects.
 * I left out the active: bool property because the GameObject already has an active property.
 * If it is not used for the same reason, make the naming more specific to avoid confusion.
 * I could not tell what it was for, so I left it out for now.
 */
class Component : public GameObject {
private:
    std::weak_ptr<GameObject> _parent;
    // TODO: Maybe add the active property back with a more specific name if needed.
public:
    explicit Component(const Scene& scene);
    ~Component() override = default;

    virtual void update() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;
    virtual void onSerialize() = 0;
    virtual void onDeserialize() = 0;
};

template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value;