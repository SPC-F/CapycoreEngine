#pragma once

#include <engine/core/rendering/renderable.h>
#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/graph.h>
#include <engine/public/components/ai/navigation/navigation_graph.h>
#include <engine/public/gameObject.h>

/**
 * @brief Component responsible for AI pathfinding using a navigation graph.
 *
 * This component generates paths for AI-controlled GameObjects to navigate
 * through the environment based on a NavigationGraph.
 *
 * NOTE: This component requires a NavigationGraph component to be present
 * on the same GameObject to function correctly.
 */
class Pathfinding : public Renderable {
 public:
  Pathfinding();

  /** @brief Update is left empty as the pathfinding component does not require
   * per-frame updates */
  void update(float dt) override{};

  /**
   * @brief Generates a path to the specified target GameObject.
   *
   * @note This can only be called if the parent GameObject has a
   * NavigationGraph component.
   *
   * @param target The target GameObject to generate a path to.
   * @return Reference to the Pathfinding instance for method chaining.
   */
  Pathfinding& generate_path_to_position(Vector3 origin, Vector3 target);
  Pathfinding& generate_path_to_position();

  /**
   * @brief Generates a path to the target GameObject using the A* algorithm.
   *
   * @note This can only be called if the parent GameObject has a
   * NavigationGraph component.
   *
   * @return Reference to the Pathfinding instance for method chaining.
   */
  Pathfinding& generate_path_to_target(Vector3 origin);
  Pathfinding& generate_path_to_target();

  /**
   * @brief Generates a path to the current target GameObject using the A*
   * algorithm.
   *
   * @note This can only be called if the parent GameObject has a
   * NavigationGraph component and a target GameObject has been set.
   *
   * @return Reference to the Pathfinding instance for method chaining.
   */
  Pathfinding& generate_path_to_target_game_object();

  /**
   * @brief Retrieves the current path of the AI agent.
   *
   * @return A constant reference to the vector of GameObject references
   * representing the path. These game objects are children of tiles in the
   * NavigationGraph and have NavigationNode components.
   */
  std::vector<std::reference_wrapper<GameObject>>& get_path();

  /**
   * @brief Finds the closest navigation node to the specified world position.
   *
   * @param world_position The world position to find the closest navigation
   * node to.
   * @return An optional GraphPosition of the closest navigation node if found,
   * std::nullopt otherwise.
   */
  std::optional<GraphPosition> find_closest_node(
      const Vector3& world_position) const;

  /// Getters and Setters for origin (positions)
  std::optional<Vector3> get_origin() const;
  Pathfinding& set_origin(Vector3 origin);

  /// Getters and Setters for target (positions)
  std::optional<Vector3> get_target() const;
  Pathfinding& set_target(Vector3 target);

  /// Getters and Setters for target (GameObject)
  std::optional<std::reference_wrapper<GameObject>> get_target_game_object()
      const;
  Pathfinding& set_target_game_object(GameObject& target);

  void on_serialize() override{};
  void on_deserialize() override{};

 private:
  std::vector<std::reference_wrapper<GameObject>> path_;
  std::optional<Vector3> origin_;
  std::optional<Vector3> target_;
  std::optional<std::reference_wrapper<GameObject>> target_game_object_;

  std::reference_wrapper<NavigationGraph> get_navigation_graph_component()
      const;
  void generate_path_a_star(GraphPosition origin, GraphPosition target);
};