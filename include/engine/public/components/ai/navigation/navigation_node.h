#pragma once

#include <engine/core/rendering/renderable.h>
#include <engine/public/component.h>
#include <engine/public/components/ai/navigation/graph.h>
#include <engine/public/gameObject.h>

#include <unordered_map>
#include <vector>

/**
 * @brief Component representing a navigation node in a navigation graph.
 *
 * This component holds references to neighboring nodes and associated movement
 * costs. It is used in AI pathfinding to navigate through a graph of nodes.
 */
class NavigationNode : public Renderable {
  using Edge = GraphEdge<NavigationNode>;

 public:
  NavigationNode();

  /** @brief Update is left empty as navigation nodes do not require per-frame
   * updates */
  void update(float dt) override{};

  /**
   * @brief Adds a neighboring node with the specified movement cost.
   *
   * @param neighbor Reference to the neighboring NavigationNode.
   * @param cost The cost associated with moving to the neighbor.
   * @return Reference to the NavigationNode instance for method chaining.
   */
  NavigationNode& add_edge(NavigationNode& neighbor, float cost);

  /**
   * @brief Retrieves the edges (connections) to neighboring nodes.
   *
   * @return A constant reference to the vector of Edge representing the edges.
   */
  const std::vector<Edge>& get_edges() const;

  /**
   * @brief Retrieves the edge to a specific neighboring node, if it exists.
   *
   * @param neighbor Reference to the neighboring NavigationNode.
   * @return An optional reference to the Edge if the edge exists, std::nullopt
   * otherwise.
   */
  std::optional<std::reference_wrapper<Edge>> get_edge_to(
      const NavigationNode& neighbor) const;

  /**
   * @brief Gets the graph position of this navigation node.
   *
   * @return The GraphPosition of the navigation node.
   */
  GraphPosition position() const noexcept;

  /**
   * @brief Sets the graph position of this navigation node.
   *
   * @param position The GraphPosition to set.
   * @return Reference to the NavigationNode instance for method chaining.
   */
  NavigationNode& position(const GraphPosition& position);

 private:
  GraphPosition position_;
  std::vector<Edge> edges_;
};