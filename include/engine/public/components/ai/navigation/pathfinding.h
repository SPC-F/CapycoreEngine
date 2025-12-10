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

  /**
   * @brief Generates a path to the target GameObject using the A* algorithm.
   *
   * @note This can only be called if the parent GameObject has a
   * NavigationGraph component.
   *
   * @return Reference to the Pathfinding instance for method chaining.
   */
  Pathfinding& generate_path_to_target(Vector3 origin);

  /**
   * @brief Retrieves the current path of the AI agent.
   *
   * @return A constant reference to the vector of GameObject references
   * representing the path.
   */
  const std::vector<std::reference_wrapper<GameObject>>& get_path() const;

  std::optional<GraphPosition> find_closest_node(
      const Vector3& world_position) const;

  std::optional<std::reference_wrapper<GameObject>> get_target() const;
  Pathfinding& set_target(GameObject& target);
  Pathfinding& clear_target();

  void on_serialize() override{};
  void on_deserialize() override{};

 private:
  std::vector<std::reference_wrapper<GameObject>> path_;
  std::optional<std::reference_wrapper<GameObject>> current_target_;

  std::reference_wrapper<NavigationGraph> get_navigation_graph_component()
      const;
  void generate_path_a_star(GraphPosition origin, GraphPosition target);
};