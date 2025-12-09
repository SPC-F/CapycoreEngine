#include <engine/public/components/ai/navigation/navigation_node.h>

NavigationNode::NavigationNode() {
  add_on_attach([this](Component& comp) {
    if (!parent().has_value()) {
      throw std::runtime_error("Collider2D has no parent GameObject.");
    }

    this->disable_draw();
    this->set_render_strategy(comp);
  });
}

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