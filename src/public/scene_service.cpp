#include <engine/public/scene_service.h>

SceneService::SceneService() : SceneService(DEFAULT_SCENE_NAME) {}
SceneService::SceneService(const std::string& initial_scene_name)
    : is_running_(true) {
  auto* scene = new Scene(initial_scene_name);
  scenes_.try_emplace(initial_scene_name,
                      std::move(std::unique_ptr<Scene>(scene)));

  current_scene_ = *scenes_.at(initial_scene_name);
}

SceneService::~SceneService() = default;

void SceneService::run_current() {
  is_running_ = true;

  current_scene_ = *scenes_.at(next_scene_name_.value_or(fallback_scene_name_));

  while (is_running_) {
    current_scene_->get().run();

    // Adding a check for is_running_ immediately after
    // current_scene_->get().run() to break the loop if the service is stopping
    // (scene swap).
    if (!is_running_) {
      break;
    }

    // Using next_scene_name_.reset() instead of blindly setting it to the
    // fallback scene name and correctly determining the target scene name
    // before resetting the optional.
    std::string to_load = next_scene_name_.value_or(fallback_scene_name_);
    next_scene_name_.reset();

    Scene& new_scene = *scenes_.at(to_load);
    move_dont_destroy_on_load_objects(new_scene);
    current_scene_ = new_scene;
  }
  current_scene_->get().stop();
}
bool SceneService::is_running() const { return is_running_; }
void SceneService::stop() { is_running_ = false; }

Scene& SceneService::add_scene(const std::string& name) {
  if (name == DEFAULT_SCENE_NAME) {
    throw std::runtime_error("Cannot add scene with reserved name '" +
                             std::string(DEFAULT_SCENE_NAME) + "'");
  }
  const auto new_scene = scenes_.find(name);

  // Scene not found
  if (new_scene == scenes_.end()) {
    auto* scene = new Scene(name);
    scenes_.try_emplace(name, std::move(std::unique_ptr<Scene>(scene)));
    return *scene;
  }

  return *(new_scene->second);
}

SceneService& SceneService::load_scene(const std::string& name) {
  if (!scenes_.contains(name)) {
    throw std::runtime_error("Scene with name '" + name + "' not found");
  }

  if (current_scene_->get().name() == name) {
    return *this;  // Scene is already loaded
  }

  next_scene_name_ = name;
  current_scene_->get().mark_for_stopping();

  return *this;
}

void SceneService::move_dont_destroy_on_load_objects(Scene& next_scene) const {
  std::vector<std::reference_wrapper<GameObject>> objects_to_move;
  Scene& current_scene = current_scene_.value();

  for (auto obj : current_scene.game_objects()) {
    // Only move root objects, hierarchy is preserved by moving the root
    if (obj.get().dont_destroy_on_load() && !obj.get().parent().has_value()) {
      objects_to_move.push_back(obj);
    }
  }

  std::function<void(GameObject&, Scene&)> update_scene_ref;
  update_scene_ref = [&](GameObject& obj, Scene& new_scene) {
    obj.scene(new_scene);
    for (auto child : obj.children()) {
      if (auto child_ptr = current_scene.extract_game_object(child)) {
        update_scene_ref(*child_ptr, new_scene);
        new_scene.add_game_object(std::move(child_ptr));
      }
    }
  };

  for (auto obj_ref : objects_to_move) {
    GameObject& obj = obj_ref.get();
    if (auto ptr = current_scene.extract_game_object(obj)) {
      update_scene_ref(*ptr, next_scene);
      next_scene.add_game_object(std::move(ptr));
    }
  }
}

SceneService& SceneService::add_scene_and_load(const std::string& name) {
  this->add_scene(name);
  this->load_scene(name);
  return *this;
}

SceneService& SceneService::remove_scene(const std::string& name) {
  if (name == DEFAULT_SCENE_NAME) {
    throw std::runtime_error("Cannot remove scene with reserved name '" +
                             std::string(DEFAULT_SCENE_NAME) + "'");
  }
  if (name == fallback_scene_name_) {
    throw std::runtime_error(
        "Scene with name '" + name +
        "' can not be removed "
        "as it is set as the fallback scene. Please select a different "
        "fallback scene before removing scene " +
        name);
  }

  const auto it = scenes_.find(name);
  if (it == scenes_.end()) {
    throw std::runtime_error("Scene with name '" + name + "' not found");
  }
  scenes_.erase(it);
  return *this;
}

std::set<std::string> SceneService::contained_scene_names() const {
  std::set<std::string> names{};

  for (const auto& scene_pair : scenes_) {
    names.insert(scene_pair.first);
  }

  return names;
}

Scene& SceneService::current_scene() const { return current_scene_.value(); }

SceneService& SceneService::set_fallback_scene(const std::string& name) {
  if (const auto it = scenes_.find(name); it == scenes_.end()) {
    throw std::runtime_error("Scene with name '" + name + "' not found");
  }
  fallback_scene_name_ = name;
  return *this;
}

const std::string& SceneService::fallback_scene() const {
  return fallback_scene_name_;
}