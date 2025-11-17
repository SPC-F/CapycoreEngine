#include <engine/public/scene_service.h>

const std::string& default_scene_name = "default";

SceneService::SceneService()
    : scene_(std::make_unique<Scene>(default_scene_name)) {}
SceneService::SceneService(std::unique_ptr<Scene> initial_scene)
    : scene_(std::move(initial_scene)) {}

SceneService::~SceneService() = default;
SceneService& SceneService::load_scene(std::unique_ptr<Scene> scene) {
    scene_ = std::move(scene);
    return *this;
}
Scene& SceneService::current_scene() const {
    return *scene_;
}