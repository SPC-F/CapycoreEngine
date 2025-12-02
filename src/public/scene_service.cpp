#include <engine/public/scene_service.h>

SceneService::SceneService() = default;
SceneService::SceneService(const std::string& name) {
  auto* scene = new Scene(name);
  scene->run();
  scenes_.try_emplace(name, std::move(std::unique_ptr<Scene>(scene)));
}

SceneService::~SceneService() = default;

Scene& SceneService::add_scene(const std::string& name) {
  auto it = scenes_.find(name);

  // Scene not found
  if (it == scenes_.end()) {
    auto* scene = new Scene(name);
    scenes_.try_emplace(name, std::move(std::unique_ptr<Scene>(scene)));
    return *scene;
  }

  return *(it->second);
}

SceneService& SceneService::load_scene(const std::string& name) {
  auto current_scene_opt = this->current_scene();

  auto it = scenes_.find(name);
  if (it == scenes_.end()) {
    throw std::runtime_error("Scene with name '" + name + "' not found");
  }

  if (!current_scene_opt.has_value()) {
    it->second->run();
    return *this;
  }

  Scene& current_scene = current_scene_opt.value().get();

  if (current_scene.name() == name) {
    return *this;  // Scene is already loaded
  }

  Scene& next_scene = *it->second;
  move_dont_destroy_on_load_objects(current_scene, next_scene);
  current_scene.stop();
  it->second->run();

  return *this;
}

void SceneService::move_dont_destroy_on_load_objects(Scene& current_scene,
                                                     Scene& next_scene) {
  std::vector<std::reference_wrapper<GameObject>> objects_to_move;
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
  auto it = scenes_.find(name);
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

std::optional<std::reference_wrapper<Scene>> SceneService::current_scene()
    const {
  for (const auto& scene_pair : scenes_) {
    if (scene_pair.second->is_running()) {
      return *(scene_pair.second);
    }
  }

  return std::nullopt;
}