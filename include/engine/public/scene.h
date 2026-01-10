#pragma once

#include <engine/public/camera.h>
#include <engine/public/gameObject.h>

#include <functional>
#include <memory>
#include <vector>

/**
 * @brief Represents a scene within the engine.
 * A scene contains multiple GameObjects and manages their lifecycle,
 * including running the game loop, stopping, and destroying the scene.
 *
 * @note A scene can have multiple cameras, but only one main camera.
 * @note GameObjects can be marked as "don't destroy on load" to persist
 * between scene loads.
 */
class Scene {
 private:
  int stop_event_listener_id_;
  const std::string name_;

  bool is_stopping_;
  bool is_running_;

  float time_scale_;

  std::vector<std::unique_ptr<GameObject>> game_objects_;

  Scene(const std::string& name);

  using listener_function_t = std::function<void(Scene&)>;
  std::vector<listener_function_t> run_listeners_;
  std::vector<listener_function_t> stop_listeners_;
  std::vector<listener_function_t> destroy_listeners_;

  void execute_listeners(const std::vector<listener_function_t>& listeners);

  void game_loop();
  friend class SceneService;
  void run();

 public:
  virtual ~Scene();

  /**
   * @brief Registers a listener function to be called when the scene starts
   * running.
   * @param listener The function to be called when the scene starts.
   */
  void on_run(listener_function_t&& listener);

  /**
   * @brief Registers a listener function to be called when the scene stops.
   * @param listener The function to be called when the scene stops.
   */
  void on_stop(listener_function_t&& listener);

  /**
   * @brief Registers a listener function to be called when the scene is
   * destroyed.
   * @param listener The function to be called when the scene is destroyed.
   */
  void on_destroy(listener_function_t&& listener);

  /**
   * @brief marks the scene to be stopped.
   */
  void stop();

  /**
   * @brief Checks if the scene is marked for stopping.
   * @return true if the scene is marked for stopping, false otherwise.
   */
  [[nodiscard]] bool marked_for_stopping() const;

  /**
   * @brief Marks the scene to be stopped.
   */
  void mark_for_stopping() noexcept;

  /**
   * @brief Sets the time scale modifier for the scene.
   * @param modifier The time scale modifier. A value of 1.0 represents normal
   * time, values greater than 1.0 speed up time, and values less than 1.0 slow
   * down time.
   * @return A reference to this Scene for chaining.
   */
  Scene& time_scale(float modifier);

  /**
   * @brief Retrieves the current time scale modifier for the scene.
   * @return The time scale modifier.
   */
  [[nodiscard]] float time_scale() const;

  /**
   * @brief Retrieves the name of the scene.
   * @return The name of the scene as a string.
   */
  [[nodiscard]] const std::string& name() const;

  /**
   * @brief Checks if the scene is currently running.
   * @return true if the scene is running, false otherwise.
   */
  [[nodiscard]] bool is_running() const;

  /**
   * @brief Retrieves a GameObject by its unique identifier.
   * @param id The unique identifier of the GameObject.
   * @return A reference to the GameObject with the specified ID.
   * @throws std::runtime_error if no GameObject with the given ID exists in the
   * scene.
   */
  [[nodiscard]] std::reference_wrapper<GameObject> get_game_object(
      const std::string& id) const;

  /**
   * @brief Retrieves all GameObjects in the scene.
   * @return A vector of references to all GameObjects in the scene.
   */
  [[nodiscard]] std::vector<std::reference_wrapper<GameObject>> game_objects()
      const;

  /**
   * @brief Retrieves all active GameObjects in the scene.
   * @return A vector of references to all active GameObjects in the scene.
   */
  [[nodiscard]] std::vector<std::reference_wrapper<GameObject>>
  active_game_objects() const;

  /**
   * @brief Adds a new GameObject of type T to the scene.
   * @tparam T The type of the GameObject to add. Must be derived from
   * GameObject
   * @param args The arguments to forward to the GameObject's constructor.
   * @return A reference to the added GameObject.
   */
  template <typename T, typename... Args>
  T& add_game_object(Args&&... args) {
    static_assert(std::is_base_of_v<GameObject, T>,
                  "T must be derived from GameObject");

    auto game_object = std::make_unique<T>(std::forward<Args>(args)...);
    game_objects_.emplace_back(std::move(game_object));

    return static_cast<T&>(*game_objects_.back());
  }

  /**
   * @brief Adds a new GameObject with the given name to the scene.
   * @param name The name of the GameObject to add.
   * @return A reference to the added GameObject.
   */
  GameObject& add_game_object(const std::string& name);

  /**
   * @brief Adds an existing GameObject to the scene.
   * @param game_object The unique pointer to the GameObject to add.
   * @return A reference to the added GameObject.
   */
  Scene& add_game_object(std::unique_ptr<GameObject> game_object);

  /**
   * @brief Adds multiple existing GameObjects to the scene.
   * @param game_objects A vector of unique pointers to the GameObjects to add.
   * @return A reference to the added GameObjects.
   */
  Scene& add_game_objects(
      std::vector<std::unique_ptr<GameObject>> game_objects);

  /**
   * @brief Extracts a GameObject from the scene without deleting it.
   * @param game_object The GameObject to extract.
   * @return A unique pointer to the extracted GameObject.
   */
  std::unique_ptr<GameObject> extract_game_object(GameObject& game_object);

  /**
   * @brief Removes a GameObject from the scene and deletes it.
   * @param game_object The GameObject to remove.
   * @return true if the GameObject was found and removed, false otherwise.
   */
  bool remove_game_object(GameObject& game_object);

  /**
   * @brief Cleans up and deletes all GameObjects that are marked for deletion.
   */
  void cleanup_destroyed_game_objects();

  /**
   * @brief Retrieves the main camera of the scene.
   * @return An optional reference to the main Camera if it exists, std::nullopt
   * otherwise.
   */
  [[nodiscard]] std::optional<std::reference_wrapper<Camera>> main_camera()
      const;
};
