#include <engine/public/components/ai/navigation/pathfinding.h>
#include <engine/public/util/point.h>

#include <algorithm>
#include <cfloat>
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

    this->disable_draw();
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

Pathfinding& Pathfinding::generate_path_to_position() {
  if (!origin_.has_value() || !target_.has_value()) {
    throw std::runtime_error(
        "Origin or target position not set for pathfinding. Cannot generate "
        "path.");
  }

  return generate_path_to_position(origin_.value(), target_.value());
}

Pathfinding& Pathfinding::generate_path_to_target(Vector3 origin) {
  if (!target_.has_value()) {
    throw std::runtime_error(
        "No target GameObject set for pathfinding. Cannot generate path.");
  }

  auto origin_pos = find_closest_node(origin);
  auto target_pos = find_closest_node(target_.value());

  if (!origin_pos.has_value() || !target_pos.has_value()) {
    throw std::runtime_error(
        "Could not find valid navigation nodes for origin or target. Cannot "
        "generate path.");
  }

  generate_path_a_star(origin_pos.value(), target_pos.value());

  return *this;
}

Pathfinding& Pathfinding::generate_path_to_target() {
  if (!origin_.has_value()) {
    throw std::runtime_error(
        "Origin position not set for pathfinding. Cannot generate path.");
  }

  return generate_path_to_target(origin_.value());
}

Pathfinding& Pathfinding::generate_path_to_target_game_object() {
  if (!target_game_object_.has_value()) {
    throw std::runtime_error(
        "No target GameObject set for pathfinding. Cannot generate path.");
  }

  if (!origin_.has_value()) {
    throw std::runtime_error(
        "Origin position not set for pathfinding. Cannot generate path.");
  }

  auto target_game_object_opt = target_game_object_;
  if (!target_game_object_opt.has_value()) {
    throw std::runtime_error(
        "No target GameObject set for pathfinding. Cannot generate path.");
  }

  auto position = target_game_object_opt->get().transform().position();
  generate_path_to_position(origin_.value(), position);

  return *this;
}

std::vector<std::reference_wrapper<GameObject>>& Pathfinding::get_path() {
  return path_;
}

void Pathfinding::generate_path_a_star(GraphPosition origin,
                                       GraphPosition target) {
  path_.clear();

  NavigationGraph& graph = get_navigation_graph_component();
  auto& nodes = graph.get_nodes();

  if (!nodes.count(origin) || !nodes.count(target))
    throw std::runtime_error("Pathfinding: origin or target node missing.");

  std::unordered_map<GraphPosition, NodeRecord, GraphPositionHash> open;
  std::unordered_set<GraphPosition, GraphPositionHash> closed;
  std::unordered_map<GraphPosition, NodeRecord, GraphPositionHash> all_records;

  auto heuristic = [&](const GraphPosition& a, const GraphPosition& b) {
    float dx = float(a.x - b.x);
    float dy = float(a.y - b.y);
    return dx * dx + dy * dy;
  };

  NodeRecord start;
  start.pos = origin;
  start.cost_from_start = 0.f;
  start.heuristic = heuristic(origin, target);
  start.has_parent = false;

  open.emplace(origin, start);
  all_records[origin] = start;

  while (!open.empty()) {
    /// Pick node with lowest f()
    auto current_it =
        std::min_element(open.begin(), open.end(), [](auto& a, auto& b) {
          return a.second.total_cost() < b.second.total_cost();
        });

    NodeRecord current = current_it->second;

    closed.insert(current.pos);
    open.erase(current_it);

    all_records[current.pos] = current;

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

      /// Extract game objects from navigation nodes
      for (auto& gp : reversed) {
        auto& nav_node = nodes.at(gp).get();
        auto parent_opt = nav_node.parent();
        if (parent_opt.has_value()) path_.push_back(parent_opt.value());
      }

      return;
    }

    NavigationNode& node = nodes.at(current.pos);
    for (auto& edge : node.get_edges()) {
      NavigationNode& neigh = edge.target.get();
      auto parent_opt = neigh.parent();
      if (!parent_opt.has_value()) continue;

      GraphPosition neigh_pos = edge.target.get().position();

      if (closed.count(neigh_pos)) continue;

      float new_cost_from_start = current.cost_from_start + edge.cost;
      auto open_it = open.find(neigh_pos);

      /// Not in open set yet or found a better path
      if (open_it == open.end()) {
        NodeRecord rec;
        rec.pos = neigh_pos;
        rec.cost_from_start = new_cost_from_start;
        rec.heuristic = heuristic(neigh_pos, target);
        rec.parent = current.pos;
        rec.has_parent = true;

        open.emplace(neigh_pos, rec);
        all_records[neigh_pos] = rec;
      } else {
        if (new_cost_from_start < open_it->second.cost_from_start) {
          open_it->second.cost_from_start = new_cost_from_start;
          open_it->second.parent = current.pos;
          open_it->second.has_parent = true;

          all_records[neigh_pos] = open_it->second;
        }
      }
    }
  }
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

std::optional<Vector3> Pathfinding::get_origin() const { return origin_; }

Pathfinding& Pathfinding::set_origin(Vector3 origin) {
  origin_ = origin;
  return *this;
}

std::optional<Vector3> Pathfinding::get_target() const { return target_; }

Pathfinding& Pathfinding::set_target(Vector3 target) {
  target_ = target;
  return *this;
}

std::optional<std::reference_wrapper<GameObject>>
Pathfinding::get_target_game_object() const {
  return target_game_object_;
}

Pathfinding& Pathfinding::set_target_game_object(GameObject& target) {
  target_game_object_ = std::ref(target);
  return *this;
}