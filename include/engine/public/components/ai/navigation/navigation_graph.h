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
  NavigationGraph(int grid_size = 16, int stride = 10);

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

  std::unordered_map<GraphPosition, std::reference_wrapper<NavigationNode>,
                     GraphPositionHash>&
  get_nodes();

  /**
   * @brief Retrieves a navigation node at the specified position.
   * @param position The position to query for a navigation node.
   * @return An optional reference to the NavigationNode if found, std::nullopt
   * otherwise.
   */
  std::optional<std::reference_wrapper<NavigationNode>> get_node(
      const GraphPosition& position) const;

  std::optional<GraphPosition> get_position_of_node(
      const NavigationNode& node) const;

  GraphPosition world_to_graph_position(const Vector3& world_position) const;

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

  std::optional<std::reference_wrapper<NavigationNode>> get_closest_node(
      const Vector3& world_position) const;

  int get_grid_size() const noexcept;
  int get_grid_max_x() const noexcept;
  int get_grid_max_y() const noexcept;

  void on_serialize() override{};
  void on_deserialize() override{};

 private:
  /// Mapping of graph positions to navigation nodes
  std::unordered_map<GraphPosition, std::reference_wrapper<NavigationNode>,
                     GraphPositionHash>
      nodes_;

  /// Mapping of graph positions to TILES (GameObjects)
  /// Used to keep track of obstacles when linking nodes
  std::unordered_map<GraphPosition, GameObject*, GraphPositionHash> tile_map_;

  /// Mapping of graph positions to NODES (NavigationNodes)
  /// Used to store Nodes on the grid in comparison to tile_map_
  std::unordered_map<GraphPosition, NavigationNode*, GraphPositionHash>
      node_tile_map_;

  int stride_ = 10;
  int max_drop_distance_ = 10;
  int max_jump_distance_ = 4;
  float node_vertical_offset_{16.0f};

  int grid_size_{16};
  int grid_max_x_{0};
  int grid_max_y_{0};

  void set_tile_maps(GameObject& parent);
  void generate_nodes();
  void link_nodes();

  std::optional<std::reference_wrapper<NavigationNode>> find_node_in_children(
      GameObject& parent) const noexcept;
};