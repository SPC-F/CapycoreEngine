#pragma once

#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/gameObject.h>

/**
 * @brief Enumeration for AI Controller modes.
 *
 * PATROL: The AI will patrol between set points.
 * CHASE: The AI will chase a specified target.
 * NOTE: Both support kinematic (non-physics) and physics-based movement using
 * the Rigidbody2D component with colliders
 */
enum AIControllerMode : uint8_t {
  PATROL = 0,
  CHASE = 1,
};

/**
 * @brief Component responsible for controlling AI behavior.
 *
 * This component utilizes the Pathfinding component to navigate the
 * AI-controlled GameObject towards a specified target GameObject.
 *
 * The AIController can operate in different modes, such as PATROL and CHASE,
 * allowing for versatile AI behaviors.
 *
 * There still is a possibility that the AI gets stuck on corners or
 * obstacles depending on the navigation graph and collider setup. It has
 * some basic stuck detection and will try to recalculate the path
 * if it detects that it's not moving.
 *
 * NOTE: for this to work you need to set up a proper navigation graph using
 * the Pathfinding component and ensure that the AI GameObject has appropriate
 * colliders (and Rigidbody2D if using physics-based movement).
 */
class AIController : public Component {
 public:
  AIController(GameObject& pathfinding_game_object);

  void update(float dt) override;

  /**
   * @brief Handles chasing behavior towards the chase target.
   */
  void chase(float dt);

  /**
   * @brief Handles patrolling behavior between patrol points.
   */
  void patrol(float dt);

  /**
   * @brief Attempts to traverse the navigation graph from source to target.
   *
   * @param source The Transform of the AI-controlled GameObject.
   * @param target The Transform of the target GameObject.
   * @param dt The delta time since the last update.
   */
  void try_traverse_graph(Transform& source, Transform& target, float dt);

  AIControllerMode get_mode() const;
  AIController& set_mode(AIControllerMode mode);

  std::optional<std::reference_wrapper<GameObject>> get_chase_target() const;
  AIController& set_chase_target(GameObject& target);
  AIController& clear_chase_target();

  std::optional<std::reference_wrapper<GameObject>> get_patrol_target() const;
  AIController& set_patrol_target(GameObject& target);
  AIController& clear_patrol_target();

  float get_arrival_threshold() const;
  AIController& set_arrival_threshold(float threshold);

  float get_speed() const;
  AIController& set_speed(float speed);

  float get_width() const;
  AIController& set_width(float width);

  float get_height() const;
  AIController& set_height(float height);

  bool enable_graph_traversal() noexcept;
  bool disable_graph_traversal() noexcept;

  size_t add_on_patrol_complete_action(
      std::function<void(AIController&)> action);
  void remove_on_patrol_complete_action(size_t index);

  void on_serialize() override{};
  void on_deserialize() override{};

 private:
  std::optional<std::reference_wrapper<GameObject>> pathfinding_game_object_;
  std::optional<std::reference_wrapper<GameObject>> chase_target_;
  std::optional<std::reference_wrapper<GameObject>> patrol_target_;

  std::optional<std::reference_wrapper<Rigidbody2D>> rigidbody_{std::nullopt};

  AIControllerMode mode_{AIControllerMode::PATROL};
  bool use_graph_traversal_{true};

  Transform initial_transform;
  bool returning_to_start_{false};

  Vector3 last_position_{0, 0, 0};
  float stuck_timer_{0.0f};
  float stuck_threshold_{1.0f};

  float node_distance_threshold_{2.0f};
  float arrival_threshold_{24.0f};

  /// Speed is different for kinematic and physics-based movement
  /// A good default for kinematic is 100.0f
  /// A good default for physics-based (with Rigidbody2D) is 10.0f
  /// Faster speeds are more likely to cause visual jittering
  float speed_{10.0f};

  /// Used to check the center of the object which is needed
  /// for accurate pathfinding and not getting stuck on corners
  float width_{10.0f};
  float height_{10.0f};

  std::reference_wrapper<Pathfinding> get_pathfinding_component();

  std::vector<std::function<void(AIController&)>> on_patrol_complete_actions_;
};