#include <engine/public/components/ai/navigation/pathfinding.h>

#include <stdexcept>

std::reference_wrapper<NavigationGraph>
Pathfinding::get_navigation_graph_component() {
  auto parent_opt = parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "Pathfinding must be attached to a GameObject before use.");
  }

  auto& parent = parent_opt->get();
  auto navigation_graph_opt = parent.get_component<NavigationGraph>();
  if (!navigation_graph_opt.has_value()) {
    throw std::runtime_error(
        "Pathfinding requires a NavigationGraph component on the parent "
        "GameObject.");
  }

  return navigation_graph_opt->get();
}

Pathfinding::Pathfinding()
    : navigation_graph_(get_navigation_graph_component()) {}

Pathfinding& Pathfinding::generate_path_to_position(GraphPosition target) {
  // implement
  return *this;
}

const std::vector<std::reference_wrapper<GameObject>>& Pathfinding::get_path()
    const {
  return path_;
}

void Pathfinding::generate_path_a_star(GameObject& target) {
  // implement
}