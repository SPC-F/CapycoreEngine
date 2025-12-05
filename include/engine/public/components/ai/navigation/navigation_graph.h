#pragma once

#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/graph.h>
#include <engine/public/components/ai/navigation/navigation_node.h>
#include <engine/public/gameObject.h>

#include <unordered_map>

/**
 * @brief Component representing a navigation graph for AI pathfinding.
 *
 * This component manages a collection of navigation nodes, allowing for the
 * generation, clearing, and querying of the navigation graph.
 */
class NavigationGraph : public Component {
 public:
  NavigationGraph(float node_distance = 1.0f);

  /** @brief Update is left empty as the navigation graph does not require
   * per-frame updates */
  void update(float dt) override{};

  /**
   * @brief Generates the navigation graph by populating it with nodes.
   *
   * Generates nodes based on the children of the parent GameObject and tries to
   * connect them based on the specified node distance.
   *
   * @return Reference to the NavigationGraph instance for method chaining.
   */
  NavigationGraph& generate_graph();

  /**
   * @brief Clears all nodes from the navigation graph.
   * @return Reference to the NavigationGraph instance for method chaining.
   */
  NavigationGraph& clear();

  /**
   * @brief Retrieves a navigation node at the specified position.
   * @param position The position to query for a navigation node.
   * @return An optional reference to the NavigationNode if found, std::nullopt
   * otherwise.
   */
  std::optional<std::reference_wrapper<NavigationNode>> get_node(
      const GraphPosition& position) const;

  /**
   * @brief Adds a navigation node at the specified position.
   * @param position The position to add the navigation node.
   * @param node The NavigationNode to add.
   * @return Reference to the NavigationGraph instance for method chaining.
   */
  NavigationGraph& add_node(const GraphPosition& position,
                            NavigationNode& node);

  /**
   * @brief Removes a navigation node at the specified position.
   * @param position The position of the navigation node to remove.
   * @return Reference to the NavigationGraph instance for method chaining.
   */
  NavigationGraph& remove_node(const GraphPosition& position);

  /**
   * @brief Finds the closest navigation node to the specified position.
   * @param position The position to find the closest navigation node to.
   * @return A reference to the closest NavigationNode.
   */
  std::optional<std::reference_wrapper<NavigationNode>> get_closest_node(
      const GraphPosition& position) const;

 private:
  std::unordered_map<GraphPosition, std::reference_wrapper<NavigationNode>,
                     GraphPositionHash>
      nodes_;
  float node_distance_;

  void generate_nodes();
  void link_nodes();
};