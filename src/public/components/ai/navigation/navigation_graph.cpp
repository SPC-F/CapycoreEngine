#include <engine/public/components/ai/navigation/navigation_graph.h>

NavigationGraph::NavigationGraph(float node_distance)
    : node_distance_(node_distance) {}

NavigationGraph& NavigationGraph::generate_graph() {
  // implement
  return *this;
}

void NavigationGraph::generate_nodes() {
  // implement
}

void NavigationGraph::link_nodes() {
  // implement
}

NavigationGraph& NavigationGraph::clear() {
  nodes_.clear();
  return *this;
}

std::optional<std::reference_wrapper<NavigationNode>> NavigationGraph::get_node(
    const GraphPosition& position) const {
  auto it = nodes_.find(position);

  if (it != nodes_.end()) {
    return std::ref(it->second.get());
  }

  return std::nullopt;
}

NavigationGraph& NavigationGraph::add_node(const GraphPosition& position,
                                           NavigationNode& node) {
  nodes_.emplace(position, std::ref(node));
  return *this;
}

NavigationGraph& NavigationGraph::remove_node(const GraphPosition& position) {
  nodes_.erase(position);
  return *this;
}

std::optional<std::reference_wrapper<NavigationNode>>
NavigationGraph::get_closest_node(const GraphPosition& position) const {
  // implement
  return std::nullopt;
}
