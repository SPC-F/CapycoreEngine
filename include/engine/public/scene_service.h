#pragma once

#include <engine/core/iEngineService.h>
#include <engine/public/scene.h>

#include <map>
#include <memory>
#include <set>

class SceneService final : public IEngineService {
 private:
  std::map<const std::string, std::unique_ptr<Scene>> scenes_;

 public:
  SceneService();
  SceneService(const SceneService&) = delete;
  SceneService& operator=(const SceneService&) = delete;

  SceneService(const std::string& initial_scene_name);
  ~SceneService() override;

  Scene& add_scene(const std::string& name);
  SceneService& load_scene(const std::string& name);
  SceneService& add_scene_and_load(const std::string& name);
  SceneService& remove_scene(const std::string& name);

  [[nodiscard]] std::set<std::string> contained_scene_names() const;
  [[nodiscard]] std::optional<std::reference_wrapper<Scene>> current_scene()
      const;
};