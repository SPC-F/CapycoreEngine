#pragma once

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
class Pathfinding : public Component {
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
  Pathfinding& generate_path_to_position(GraphPosition target);

  /**
   * @brief Retrieves the current path of the AI agent.
   *
   * @return A constant reference to the vector of GameObject references
   * representing the path.
   */
  const std::vector<std::reference_wrapper<GameObject>>& get_path() const;

 private:
  std::reference_wrapper<NavigationGraph> navigation_graph_;
  std::vector<std::reference_wrapper<GameObject>> path_;

  std::reference_wrapper<NavigationGraph> get_navigation_graph_component();
  void generate_path_a_star(GameObject& target);
};