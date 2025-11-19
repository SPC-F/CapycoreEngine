#pragma once

#include <memory>
#include <engine/public/behavior.h>
#include <engine/public/component.h>

/**
 * @brief A Component that wraps a Behavior, allowing it to be attached to GameObjects.
 * 
 * This class acts as a bridge between the Behavior system and the Component system,
 * enabling Behaviors to be used as Components within the engine's architecture.
 */
class BehaviorScript : public Component
{
public:
    BehaviorScript(std::unique_ptr<Behavior> behavior);
    ~BehaviorScript() override;

    void update(float dt) override;

    void on_serialize() override;
    void on_deserialize() override;

    Behavior& behavior();

    [[nodiscard]] bool enabled() const;
    BehaviorScript& enabled(bool value);

private:
    bool started_ {false};
    std::unique_ptr<Behavior> behavior_;
};