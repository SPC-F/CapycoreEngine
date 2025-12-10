#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/util/point.h>

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <unordered_set>

std::reference_wrapper<NavigationGraph>
Pathfinding::get_navigation_graph_component() const {
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

Pathfinding::Pathfinding() {
  add_on_attach([this](Component& comp) {
    get_navigation_graph_component();
    this->set_render_strategy(comp);
  });
}

Pathfinding& Pathfinding::generate_path_to_position(Vector3 origin,
                                                    Vector3 target) {
  auto origin_pos = find_closest_node(origin);
  auto target_pos = find_closest_node(target);

  if (!origin_pos.has_value() || !target_pos.has_value()) {
    throw std::runtime_error(
        "Could not find valid navigation nodes for origin or target. Cannot "
        "generate path.");
  }

  generate_path_a_star(origin_pos.value(), target_pos.value());

  return *this;
}

Pathfinding& Pathfinding::generate_path_to_target(Vector3 origin) {
  if (!current_target_.has_value()) {
    throw std::runtime_error(
        "No target GameObject set for pathfinding. Cannot generate path.");
  }

  auto origin_pos = find_closest_node(origin);
  auto target_pos =
      find_closest_node(current_target_.value().get().transform().position());

  if (!origin_pos.has_value() || !target_pos.has_value()) {
    throw std::runtime_error(
        "Could not find valid navigation nodes for origin or target. Cannot "
        "generate path.");
  }

  generate_path_a_star(origin_pos.value(), target_pos.value());

  return *this;
}

const std::vector<std::reference_wrapper<GameObject>>& Pathfinding::get_path()
    const {
  return path_;
}

void Pathfinding::generate_path_a_star(GraphPosition origin,
                                       GraphPosition target) {
  path_.clear();
  NavigationGraph& graph = get_navigation_graph_component();
  auto& nodes = graph.get_nodes();

  if (!nodes.count(origin) || !nodes.count(target))
    throw std::runtime_error("Pathfinding: origin or target node missing.");

  struct NodeRecord {
    GraphPosition pos;
    float g = FLT_MAX;
    float h = 0.f;
    GraphPosition parent;
    bool has_parent = false;

    float f() const { return g + h; }
  };

  std::unordered_map<GraphPosition, NodeRecord, GraphPositionHash> open;
  std::unordered_set<GraphPosition, GraphPositionHash> closed;
  std::unordered_map<GraphPosition, NodeRecord, GraphPositionHash> all_records;

  auto heuristic = [&](const GraphPosition& a, const GraphPosition& b) {
    return Point(a.x, a.y).distance_to(Point(b.x, b.y)).x;
  };

  // Initialize start node
  NodeRecord start;
  start.pos = origin;
  start.g = 0.f;
  start.h = heuristic(origin, target);
  start.has_parent = false;

  open.emplace(origin, start);
  all_records[origin] = start;

  // ---- A* MAIN LOOP ----
  while (!open.empty()) {
    // Pick node with lowest f-cost
    auto current_it = std::min_element(
        open.begin(), open.end(),
        [](auto& a, auto& b) { return a.second.f() < b.second.f(); });

    NodeRecord current = current_it->second;
    // Move current node from open to closed
    closed.insert(current.pos);
    open.erase(current_it);

    // Store current BEFORE goal check so reconstruction is safe
    all_records[current.pos] = current;

    // Goal reached — reconstruct path
    if (current.pos == target) {
      std::vector<GraphPosition> reversed;
      GraphPosition trace = current.pos;

      while (true) {
        reversed.push_back(trace);

        const NodeRecord& r = all_records.at(trace);
        if (!r.has_parent) break;

        trace = r.parent;
      }

      std::reverse(reversed.begin(), reversed.end());

      // Convert positions → GameObjects
      for (auto& gp : reversed) {
        auto& nav_node = nodes.at(gp).get();
        auto parent_opt = nav_node.parent();
        if (parent_opt.has_value()) path_.push_back(parent_opt.value());
      }

      return;
    }

    // Expand neighbors
    NavigationNode& node = nodes.at(current.pos);
    for (auto& edge : node.get_edges()) {
      NavigationNode& neigh = edge.target.get();
      auto parent_opt = neigh.parent();
      if (!parent_opt.has_value()) continue;

      GraphPosition neigh_pos = edge.target.get().position();

      if (closed.count(neigh_pos)) continue;

      float new_g = current.g + edge.cost;

      auto open_it = open.find(neigh_pos);

      if (open_it == open.end()) {
        // First discovery
        NodeRecord rec;
        rec.pos = neigh_pos;
        rec.g = new_g;
        rec.h = heuristic(neigh_pos, target);
        rec.parent = current.pos;
        rec.has_parent = true;

        open.emplace(neigh_pos, rec);
        all_records[neigh_pos] = rec;  // <-- always stored
      } else {
        // Found a better route
        if (new_g < open_it->second.g) {
          open_it->second.g = new_g;
          open_it->second.parent = current.pos;
          open_it->second.has_parent = true;

          all_records[neigh_pos] = open_it->second;  // IMPORTANT
        }
      }
    }
  }

  throw std::runtime_error("A* failed to find a path.");
}

std::optional<GraphPosition> Pathfinding::find_closest_node(
    const Vector3& world_position) const {
  auto navigation_graph_ = get_navigation_graph_component();

  auto closest_node_opt =
      navigation_graph_.get().get_closest_node(world_position);
  if (!closest_node_opt.has_value()) {
    throw std::runtime_error(
        "No navigation node found close to the target position. Cannot "
        "generate path.");
  }

  auto& target_node = closest_node_opt->get();
  auto target_node_pos_opt =
      navigation_graph_.get().get_position_of_node(target_node);
  if (!target_node_pos_opt.has_value()) {
    throw std::runtime_error(
        "Failed to retrieve position of the closest navigation node.");
  }

  return target_node_pos_opt;
}

std::optional<std::reference_wrapper<GameObject>> Pathfinding::get_target()
    const {
  return current_target_;
}

Pathfinding& Pathfinding::set_target(GameObject& target) {
  current_target_ = std::ref(target);
  return *this;
}

Pathfinding& Pathfinding::clear_target() {
  current_target_ = std::nullopt;
  return *this;
}