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

    if(!current_scene_opt.has_value()) {
        it->second->run();
        return *this;
    }

    Scene& current_scene = current_scene_opt.value().get();

    if(current_scene.name() == name) {
        return *this; // Scene is already loaded
    }

    current_scene.stop();
    it->second->run();

    return *this;
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
    std::set<std::string> names {};

    for (const auto& scene_pair : scenes_) {
        names.insert(scene_pair.first);
    }

    return names;
}

std::optional<std::reference_wrapper<Scene>> SceneService::current_scene() const {
    auto current_scene = scenes_.begin();
    for (const auto& scene_pair : scenes_) {
        if (scene_pair.second->is_running()) {
            current_scene = scenes_.find(scene_pair.first);
            break;
        }
    }

    if(current_scene == scenes_.end()) {
        return std::nullopt;
    }

    return *(current_scene->second);
}