#pragma once

#include <engine/core/iEngineService.h>
#include <engine/public/scene.h>

#include <map>
#include <memory>
#include <set>

/**
 * @brief Service responsible for managing scenes within the engine.
 * This service allows for adding, removing, loading, and running scenes.
 * It also handles the transition of GameObjects marked as "don't destroy on
 * load" between scenes.
 *
 * @note Only one scene can be active and running at a time.
 * @note The SceneService must be started by calling run_current() to begin
 * the game loop of the current scene.
 * @note The SceneService can be stopped by calling stop(), which will stop
 * the current scene and exit the game loop.
 */
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

  /**
   * @brief Adds a new scene with the given name.
   * @param name The name of the scene to add.
   * @return A reference to the newly added Scene.
   */
  Scene& add_scene(const std::string& name);

  /**
   * @brief set the scene with the given name as the next scene to load.
   * The scene will be loaded at the end of the current scene's game loop.
   * @param name The name of the scene to load.
   * @return A reference to this SceneService for chaining.
   */
  SceneService& load_scene(const std::string& name);

  /**
   * @brief Adds a new scene with the given name and sets it as the next scene
   * to load. The scene will be loaded at the end of the current scene's game
   * loop.
   * @param name The name of the scene to add and load.
   * @return A reference to this SceneService for chaining.
   */
  SceneService& add_scene_and_load(const std::string& name);

  /**
   * @brief Removes the scene with the given name.
   * @param name The name of the scene to remove.
   * @return A reference to this SceneService for chaining.
   */
  SceneService& remove_scene(const std::string& name);

  /**
   * @brief Checks if a scene with the given name exists.
   * @param name The name of the scene to check.
   * @return true if the scene exists, false otherwise.
   */
  [[nodiscard]] std::set<std::string> contained_scene_names() const;

  /**
   * @brief Gets the current active scene.
   * @return A reference to the current Scene.
   * @throws std::runtime_error if no scene is currently active.
   */
  [[nodiscard]] Scene& current_scene() const;

  /**
   * @brief Sets the fallback scene name.
   * The fallback scene is loaded if the requested scene to load does not exist.
   * @param name The name of the fallback scene.
   * @return A reference to this SceneService for chaining.
   */
  SceneService& set_fallback_scene(const std::string& name);

  /**
   * @brief Gets the fallback scene name.
   * @return The name of the fallback scene.
   */
  [[nodiscard]] const std::string& fallback_scene() const;
};