#include <engine/public/components/ai/navigation/navigation_graph.h>
#include <engine/public/scene.h>

#include <cmath>

NavigationGraph::NavigationGraph(int grid_size, int stride)
    : grid_size_(grid_size), stride_(stride) {}

NavigationGraph& NavigationGraph::generate_graph() {
  generate_nodes();
  link_nodes();

  return *this;
}

std::optional<std::reference_wrapper<NavigationNode>>
NavigationGraph::find_node_in_children(GameObject& parent) const noexcept {
  for (auto& child_ref : parent.children()) {
    GameObject& child = child_ref.get();

    auto node_opt = child.get_component<NavigationNode>();
    if (node_opt.has_value()) {
      return node_opt;
    }
  }

  return std::nullopt;
}

void NavigationGraph::set_tile_maps(GameObject& parent) {
  tile_map_.clear();
  node_tile_map_.clear();
  auto& tiles = parent.children();

  // determine grid size (bounds)
  for (auto& tile_ref : tiles) {
    GameObject& tile = tile_ref.get();
    Vector3 pos = tile.transform().position();
    int gx =
        static_cast<int>(std::lround(pos.x / static_cast<float>(grid_size_)));
    int gy =
        static_cast<int>(std::lround(pos.y / static_cast<float>(grid_size_)));
    grid_max_x_ = std::max(grid_max_x_, gx) + 1;
    grid_max_y_ = std::max(grid_max_y_, gy) + 1;
  }

  // Initialize full grid maps with nullptr
  for (int y = 0; y <= grid_max_y_; ++y) {
    for (int x = 0; x <= grid_max_x_; ++x) {
      GraphPosition gpos{x, y};
      tile_map_[gpos] = nullptr;
      node_tile_map_[gpos] = nullptr;
    }
  }

  // Fill tile_map_ with actual tiles
  for (auto& tile_ref : tiles) {
    GameObject& tile = tile_ref.get();
    Vector3 pos = tile.transform().position();
    int gx =
        static_cast<int>(std::lround(pos.x / static_cast<float>(grid_size_)));
    int gy =
        static_cast<int>(std::lround(pos.y / static_cast<float>(grid_size_)));
    GraphPosition gpos{gx, gy};
    tile_map_[gpos] = &tile;
  }
}

void NavigationGraph::generate_nodes() {
  clear();

  auto parent_opt = parent();
  if (!parent_opt.has_value()) return;
  GameObject& parent_obj = parent_opt->get();
  auto& tiles = parent_obj.children();

  set_tile_maps(parent_obj);

  // Generate nodes
  for (int y = 0; y <= grid_max_y_; ++y) {
    for (int x = 0; x <= grid_max_x_; ++x) {
      GraphPosition gpos{x, y};
      if (tile_map_[gpos] == nullptr) continue;

      GameObject* tile = tile_map_[gpos];

      GraphPosition above_pos{gpos.x, gpos.y - 1};
      GraphPosition left_pos{gpos.x - 1, gpos.y - 1};
      GraphPosition right_pos{gpos.x + 1, gpos.y - 1};

      bool above = tile_map_.find(above_pos) != tile_map_.end() &&
                   tile_map_[above_pos] == nullptr;
      bool left = tile_map_.find(left_pos) != tile_map_.end() &&
                  tile_map_[left_pos] == nullptr;
      bool right = tile_map_.find(right_pos) != tile_map_.end() &&
                   tile_map_[right_pos] == nullptr;

      auto create_node = [this, &parent_obj, &tile](const GraphPosition& pos,
                                                    float x, float y) {
        GameObject& node_obj =
            parent_obj.scene().add_game_object("NavigationNode_");
        tile->add_child(node_obj);

        Vector3 node_local{};
        node_local.x = x;
        node_local.y = y;
        node_local.z = 0.0f;
        node_obj.transform().local_position(node_local);

        NavigationNode& nav_node = node_obj.add_component<NavigationNode>();
        nodes_.emplace(pos, nav_node);
        node_tile_map_[pos] = &nav_node;
      };

      if (above)
        create_node(above_pos, grid_size_ * 0.5f, -node_vertical_offset_);
      if (above && left)
        create_node(left_pos, -grid_size_ * 0.5f, -node_vertical_offset_);
      if (above && right)
        create_node(right_pos, grid_size_ * 1.5f, -node_vertical_offset_);
    }
  }
}

void NavigationGraph::link_nodes() {
  if (nodes_.empty()) return;

  // Directions for horizontal scanning
  const std::array<int, 2> horiz_offsets = {-1, 1};

  for (auto& kv : nodes_) {
    const GraphPosition& gpos = kv.first;
    NavigationNode& node = kv.second.get();

    for (int dx : horiz_offsets) {
      int step = 1;
      while (true) {
        GraphPosition check_pos{gpos.x + dx * step, gpos.y};

        if (node_tile_map_.find(check_pos) == node_tile_map_.end()) break;
        if (tile_map_.find(check_pos) != tile_map_.end() &&
            tile_map_[check_pos] != nullptr)
          break;

        if (node_tile_map_[check_pos] != nullptr) {
          node.add_edge(*node_tile_map_[check_pos], 1.0f);
          break;
        }

        ++step;
      }
    }

    for (int dy = 1; dy <= max_drop_distance_; ++dy) {
      GraphPosition check_pos{gpos.x, gpos.y + dy};  // +y => down

      if (node_tile_map_.find(check_pos) == node_tile_map_.end()) break;
      if (tile_map_.find(check_pos) != tile_map_.end() &&
          tile_map_[check_pos] != nullptr)
        break;

      if (node_tile_map_[check_pos] != nullptr) {
        node.add_edge(*node_tile_map_[check_pos], 1.0f + dy * 0.5f);
        break;
      }
    }
  }
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
  float closest_distance = std::numeric_limits<float>::max();
  NavigationNode* closest_node = nullptr;

  for (const auto& [pos, node_ref] : nodes_) {
    float dx = static_cast<float>(pos.x - position.x);
    float dy = static_cast<float>(pos.y - position.y);
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < closest_distance) {
      closest_distance = distance;
      closest_node = &node_ref.get();
    }
  }

  if (closest_node) {
    return std::ref(*closest_node);
  }

  return std::nullopt;
}
