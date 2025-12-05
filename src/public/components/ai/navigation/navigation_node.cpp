#include <engine/public/components/ai/navigation/navigation_node.h>

NavigationNode::NavigationNode() = default;

NavigationNode& NavigationNode::add_edge(NavigationNode& neighbor, float cost) {
  GraphEdge<NavigationNode> edge{
      .target = std::ref(neighbor), .source = std::ref(*this), .cost = cost};

  edges_.push_back(edge);

  return *this;
}

const std::vector<NavigationNode::Edge>& NavigationNode::get_edges() const {
  return edges_;
}

std::optional<std::reference_wrapper<NavigationNode::Edge>>
NavigationNode::get_edge_to(const NavigationNode& neighbor) const {
  for (const auto& edge : edges_) {
    if (&edge.target.get() == &neighbor) {
      return std::ref(const_cast<NavigationNode::Edge&>(edge));
    }
  }

  return std::nullopt;
}
