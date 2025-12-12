#pragma once

#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/components/rigidbody_2d.h>
#include <engine/public/gameObject.h>

enum AIControllerMode : uint8_t {
  PATROL = 0,
  CHASE = 1,
};

/**
 * @brief Component responsible for controlling AI behavior.
 *
 * This component utilizes the Pathfinding component to navigate the
 * AI-controlled GameObject towards a specified target GameObject.
 */
class AIController : public Component {
 public:
  AIController(GameObject& pathfinding_game_object);

  void update(float dt) override;

  void chase(float dt);
  void patrol(float dt);
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

  float get_path_recalculation_interval() const;
  AIController& set_path_recalculation_interval(float interval);

  float get_width() const;
  AIController& set_width(float width);

  float get_height() const;
  AIController& set_height(float height);

  bool enable_graph_traversal() noexcept;
  bool disable_graph_traversal() noexcept;

  void on_serialize() override{};
  void on_deserialize() override{};

 private:
  std::optional<std::reference_wrapper<GameObject>> pathfinding_game_object_;
  std::optional<std::reference_wrapper<GameObject>> chase_target_;
  std::optional<std::reference_wrapper<GameObject>> patrol_target_;

  std::optional<std::reference_wrapper<Rigidbody2D>> rigidbody_{std::nullopt};

  AIControllerMode mode_{AIControllerMode::PATROL};
  bool use_graph_traversal_{true};

  Vector3 last_position_{0, 0, 0};
  float stuck_timer_ = 0.0f;

  float arrival_threshold_{16.0f};
  float path_recalculation_interval_{1.0f};
  float speed_{10.0f};

  float width_{10.0f};
  float height_{10.0f};

  std::reference_wrapper<Pathfinding> get_pathfinding_component();
};