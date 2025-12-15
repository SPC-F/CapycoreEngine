#include <engine/public/components/ai/ai_controller.h>

#include <algorithm>
#include <stdexcept>

std::reference_wrapper<Pathfinding> AIController::get_pathfinding_component() {
  auto parent_opt = parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "AIController must be attached to a GameObject before use.");
  }

  if (!pathfinding_game_object_.has_value()) {
    throw std::runtime_error(
        "AIController requires a Pathfinding GameObject to be set before use.");
  }

  auto pathfinding_opt =
      pathfinding_game_object_->get().get_component<Pathfinding>();
  if (!pathfinding_opt.has_value()) {
    throw std::runtime_error(
        "AIController requires a Pathfinding component on the parent "
        "GameObject.");
  }

  return pathfinding_opt->get();
}

AIController::AIController(GameObject& pathfinding_game_object)
    : pathfinding_game_object_(pathfinding_game_object) {
  add_on_attach([this](Component& comp) {
    rigidbody_ = parent().value().get().get_component<Rigidbody2D>();
    initial_transform = parent().value().get().transform();

    get_pathfinding_component();
  });
}

void AIController::update(float dt) {
  switch (mode_) {
    case AIControllerMode::CHASE:
      chase(dt);
      break;
    case AIControllerMode::PATROL:
      patrol(dt);
      break;
    default:
      throw std::runtime_error("Unknown AIControllerMode in update.");
  }
}

void AIController::chase(float dt) {
  if (!chase_target_.has_value()) return;

  auto& current_transform = parent().value().get().transform();
  auto& target_transform = chase_target_.value().get().transform();

  /// Use thee graph or ignore and fly directly towards target
  if (use_graph_traversal_)
    try_traverse_graph(current_transform, target_transform, dt);
  else {
    Vector3 half_size(width_ * 0.5f, height_ * 0.5f, 0.0f);

    Vector3 source_center = current_transform.position() + half_size;
    Vector3 target_center = target_transform.position();

    Vector3 delta = target_center - source_center;
    float dist = delta.length();

    if (dist < arrival_threshold_) return;

    Vector3 direction = delta / dist;

    if (rigidbody_.has_value()) {
      auto& rb = rigidbody_->get();

      Vector3 velocity(direction.x * speed_, direction.y * speed_,
                       direction.z * speed_);
      rb.velocity(velocity);

      return;
    }
  }
}

void AIController::patrol(float dt) {
  if (!patrol_target_.has_value()) return;

  auto& current_transform = parent().value().get().transform();
  auto& target_transform = patrol_target_.value().get().transform();

  if (returning_to_start_) {
    try_traverse_graph(current_transform, initial_transform, dt);
    return;
  }

  try_traverse_graph(current_transform, target_transform, dt);
}

void AIController::try_traverse_graph(Transform& source, Transform& target,
                                      float dt) {
  auto& pathfinding = get_pathfinding_component().get();
  auto& path = pathfinding.get_path();

  /// Apply some offset to center the AI when navigating
  Vector3 half_size(width_ * 0.5f, height_ * 0.5f, 0.0f);

  Vector3 source_center = source.position() + half_size;
  Vector3 target_center = target.position();

  float dist_to_target = (source_center - target_center).length();
  if (dist_to_target < arrival_threshold_) {
    path.clear();

    if (mode_ == AIControllerMode::PATROL)
      returning_to_start_ = !returning_to_start_;

    return;
  }

  if (path.empty()) {
    pathfinding.set_origin(source.position());
    pathfinding.set_target(target.position());

    if (mode_ == AIControllerMode::PATROL && returning_to_start_) {
      pathfinding.set_target(initial_transform.position());
    }

    pathfinding.generate_path_to_target();
  }

  if (path.empty()) return;

  auto& next_node = path.front().get();
  Vector3 next_center = next_node.transform().position();

  Vector3 delta = next_center - source_center;
  float dist = delta.length();

  if (dist < node_distance_threshold_) {
    path.erase(path.begin());
    return;
  }

  Vector3 direction = delta / dist;

  if (rigidbody_.has_value()) {
    auto& rb = rigidbody_->get();

    if ((source_center - last_position_).length() < 0.1f) {
      stuck_timer_ += dt;
      if (stuck_timer_ > stuck_threshold_) {
        path.clear();
        stuck_timer_ = 0.0f;
      }
    } else {
      stuck_timer_ = 0.0f;
    }

    last_position_ = source_center;

    Vector3 node_pos = next_node.transform().position();
    Vector3 delta = node_pos - source_center;

    Vector3 direction = delta;
    direction.normalize();

    float effective_speed = speed_;
    if (dist < 0.4f) effective_speed *= dist;

    Vector3 velocity(direction.x * effective_speed,
                     direction.y * effective_speed,
                     direction.z * effective_speed);
    rb.velocity(velocity);

    return;
  }

  source_center += direction * speed_ * dt;
  source.position(source_center - half_size);
}

AIControllerMode AIController::get_mode() const { return mode_; }

AIController& AIController::set_mode(AIControllerMode mode) {
  mode_ = mode;
  return *this;
}

std::optional<std::reference_wrapper<GameObject>>
AIController::get_chase_target() const {
  return chase_target_;
}

AIController& AIController::set_chase_target(GameObject& target) {
  chase_target_ = target;
  return *this;
}

AIController& AIController::clear_chase_target() {
  chase_target_ = std::nullopt;
  return *this;
}

std::optional<std::reference_wrapper<GameObject>>
AIController::get_patrol_target() const {
  return patrol_target_;
}

AIController& AIController::set_patrol_target(GameObject& target) {
  patrol_target_ = target;
  return *this;
}

AIController& AIController::clear_patrol_target() {
  patrol_target_ = std::nullopt;
  return *this;
}

float AIController::get_arrival_threshold() const { return arrival_threshold_; }

AIController& AIController::set_arrival_threshold(float threshold) {
  arrival_threshold_ = threshold;
  return *this;
}

float AIController::get_speed() const { return speed_; }

AIController& AIController::set_speed(float speed) {
  speed_ = speed;
  return *this;
}

float AIController::get_width() const { return width_; }

AIController& AIController::set_width(float width) {
  width_ = width;
  return *this;
}

float AIController::get_height() const { return height_; }

AIController& AIController::set_height(float height) {
  height_ = height;
  return *this;
}

bool AIController::enable_graph_traversal() noexcept {
  use_graph_traversal_ = true;
  return use_graph_traversal_;
}

bool AIController::disable_graph_traversal() noexcept {
  use_graph_traversal_ = false;
  return use_graph_traversal_;
}