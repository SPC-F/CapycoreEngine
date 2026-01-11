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
 *
 *
 * Usage:
 * - Create a BehaviorScript with a specific Behavior.
 * - Attach the BehaviorScript to a GameObject.
 * The Behavior's lifecycle methods (on_awake, on_start, on_update, on_destroy)
 * will be called automatically by the engine.
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

  virtual void on_serialize(std::vector<uint8_t>& out) const {
    behavior_->on_serialize(out);
  };
  virtual void on_deserialize(const std::vector<uint8_t>& data,
                              size_t& offset) {
    behavior_->on_deserialize(data, offset);
  };

 private:
  bool started_{false};
  std::unique_ptr<Behavior> behavior_;
};