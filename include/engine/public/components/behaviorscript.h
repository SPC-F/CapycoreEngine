#pragma once

#include <engine/public/behavior.h>
#include <engine/public/component.h>

#include <memory>

/**
 * @brief Component that hosts a Behavior.
 *
 * This class acts as a bridge between the Behavior system and the Component
 * system, enabling Behaviors to be used as Components within the engine's
 * architecture.
 *
 * @note Users typically do not interact with this class directly. Instead,
 *       they create custom Behaviors and attach them to GameObjects via
 *       Components.
 * @see Behavior
 */
class BehaviorScript : public Component {
 public:
  BehaviorScript(std::unique_ptr<Behavior> behavior);

  void on_attach() override;
  void update(float dt) override;
  void on_detach() override;

  Behavior& behavior();

  [[nodiscard]] bool enabled() const;
  BehaviorScript& enable();
  BehaviorScript& disable();

  std::string type_name() const override;

 private:
  bool started_{false};
  std::unique_ptr<Behavior> behavior_;
};