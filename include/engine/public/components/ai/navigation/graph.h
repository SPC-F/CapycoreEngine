#pragma once

#include <cfloat>
#include <functional>

/**
 * @brief Structure representing an edge in the navigation graph.
 *
 * @param T The type of the nodes connected by the edge.
 * @param target Reference to the target node of the edge.
 * @param source Reference to the source node of the edge.
 * @param cost The cost associated with traversing the edge.
 */
template <typename T>
struct GraphEdge {
  std::reference_wrapper<T> target;
  std::reference_wrapper<T> source;
  float cost;
};

/**
 * @brief Structure representing a position in the navigation graph.
 *
 * @param x The x-coordinate of the graph position.
 * @param y The y-coordinate of the graph position.
 *
 * @return bool operator== Compares two GraphPosition instances for equality.
 */
struct GraphPosition {
  int x, y;

  bool operator==(const GraphPosition& other) const noexcept {
    return x == other.x && y == other.y;
  }
};

/**
 * @brief Hash function for GraphPosition to be used in unordered containers.
 * Combines the hash values of the x and y coordinates to produce a unique hash.
 */
struct GraphPositionHash {
  size_t operator()(const GraphPosition& p) const noexcept {
    return (std::hash<int>()(p.x) << 1) ^ std::hash<int>()(p.y);
  }
};

/**
 * @brief Record structure used in A* pathfinding algorithm.
 *
 * @param pos The graph position of the node.
 * @param g The cost from the start node to this node.
 * @param h The heuristic cost estimate from this node to the target node.
 * @param parent The graph position of the parent node in the path.
 * @param has_parent Flag indicating whether this node has a parent.
 *
 * @return f() The total estimated cost (f) from start to target through this
 * node.
 */
struct NodeRecord {
  GraphPosition pos;
  float cost_from_start = FLT_MAX;
  float heuristic = 0.f;
  GraphPosition parent;
  bool has_parent = false;

  float total_cost() const noexcept { return cost_from_start + heuristic; }
};
