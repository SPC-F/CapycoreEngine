#pragma once

#include <engine/core/iEngineService.h>
#include <engine/public/scene.h>

#include <map>
#include <memory>
#include <set>

class SceneService final : public IEngineService {
 private:
  std::map<const std::string, std::unique_ptr<Scene>> scenes_;
  void move_dont_destroy_on_load_objects(Scene& next_scene) const;

  std::optional<std::reference_wrapper<Scene>> current_scene_;
  std::optional<std::string> next_scene_name_;
  std::string fallback_scene_name_ = DEFAULT_SCENE_NAME;
  bool is_running_;

 public:
  static constexpr auto DEFAULT_SCENE_NAME = "DefaultScene";

  SceneService();
  explicit SceneService(const std::string& initial_scene_name);
  SceneService(const SceneService&) = delete;
  SceneService& operator=(const SceneService&) = delete;
  ~SceneService() override;

  /**
   * @brief
   * This method runs the current scene its game loop directly.
   * @remarks
   * Only call this function when selecting an initial scene to run.
   * Use SceneService::load_scene and SceneService::add_scene_and_load
   * to switch between active scenes.
   *
   * @warning
   * Never call this method more than once, and never call it from
   * within another scene's game loop. This method runs a new game loop
   * directly on the thread where it's called from, meaning it is blocking
   * and stack-frames will pile up if called recursively.
   */
  void run_current();
  /**
   * @brief Shows if the SceneService is currently running scenes.
   */
  bool is_running() const;
  /**
   * @brief Stops the SceneService and the current scene.
   */
  void stop();

  Scene& add_scene(const std::string& name);
  SceneService& load_scene(const std::string& name);
  SceneService& add_scene_and_load(const std::string& name);
  SceneService& remove_scene(const std::string& name);

  [[nodiscard]] std::set<std::string> contained_scene_names() const;
  [[nodiscard]] Scene& current_scene() const;

  SceneService& set_fallback_scene(const std::string& name);
  [[nodiscard]] const std::string& fallback_scene() const;
};