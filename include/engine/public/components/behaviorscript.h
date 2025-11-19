#pragma once

#include <memory>
#include <engine/core/behavior.h>
#include <engine/public/component.h>

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