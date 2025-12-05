#include <engine/public/components/ai/ai_controller.h>

#include <stdexcept>

std::reference_wrapper<Pathfinding> AIController::get_pathfinding_component() {
  auto parent_opt = parent();
  if (!parent_opt.has_value()) {
    throw std::runtime_error(
        "AIController must be attached to a GameObject before use.");
  }

  auto& parent = parent_opt->get();
  auto pathfinding_opt = parent.get_component<Pathfinding>();
  if (!pathfinding_opt.has_value()) {
    throw std::runtime_error(
        "AIController requires a Pathfinding component on the parent "
        "GameObject.");
  }

  return pathfinding_opt->get();
}

AIController::AIController() : pathfinding_(get_pathfinding_component()) {}

void AIController::update(float dt) {
  // implement
}

AIController& AIController::set_target(GameObject& target) {
  target_ = std::ref(target);
  return *this;
}

std::optional<std::reference_wrapper<GameObject>> AIController::get_target()
    const {
  return target_;
}

AIController& AIController::clear_target() {
  target_ = std::nullopt;
  return *this;
}
