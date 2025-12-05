#pragma once

#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/gameObject.h>

/**
 * @brief Component responsible for controlling AI behavior.
 *
 * This component utilizes the Pathfinding component to navigate the
 * AI-controlled GameObject towards a specified target GameObject.
 */
class AIController : public Component {
 public:
  AIController();

  void update(float dt) override;

  /**
   * @brief Sets the target GameObject for the AI to navigate towards.
   * @param target The target GameObject.
   * @return Reference to the AIController instance for method chaining.
   */
  AIController& set_target(GameObject& target);

  /**
   * @brief Retrieves the current target GameObject, if set.
   * @return An optional reference to the target GameObject.
   */
  std::optional<std::reference_wrapper<GameObject>> get_target() const;

  /**
   * @brief Clears the current target GameObject.
   * @return Reference to the AIController instance for method chaining.
   */
  AIController& clear_target();

 private:
  std::reference_wrapper<Pathfinding> pathfinding_;
  std::optional<std::reference_wrapper<GameObject>> target_;

  std::reference_wrapper<Pathfinding> get_pathfinding_component();
};