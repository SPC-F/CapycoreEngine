#pragma once

#include <engine/public/camera.h>
#include <engine/public/gameObject.h>

#include <functional>
#include <memory>
#include <vector>

class Scene {
 private:
  int stop_event_listener_id_;
  const std::string name_;
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

  void on_run(listener_function_t&& listener);
  void on_stop(listener_function_t&& listener);
  void on_destroy(listener_function_t&& listener);

  void stop();

  Scene& time_scale(float modifier);
  [[nodiscard]] float time_scale() const;

  [[nodiscard]] const std::string& name() const;
  [[nodiscard]] bool is_running() const;

  [[nodiscard]] std::reference_wrapper<GameObject> get_game_object(
      const std::string& id) const;
  [[nodiscard]] std::vector<std::reference_wrapper<GameObject>> game_objects()
      const;
  [[nodiscard]] std::vector<std::reference_wrapper<GameObject>>
  active_game_objects() const;

  template <typename T, typename... Args>
  T& add_game_object(Args&&... args) {
    static_assert(std::is_base_of_v<GameObject, T>,
                  "T must be derived from GameObject");

    auto game_object = std::make_unique<T>(std::forward<Args>(args)...);
    game_objects_.emplace_back(std::move(game_object));

    return static_cast<T&>(*game_objects_.back());
  }
  GameObject& add_game_object(const std::string& name);

  Scene& add_game_object(std::unique_ptr<GameObject> game_object);
  Scene& add_game_objects(
      std::vector<std::unique_ptr<GameObject>> game_objects);

  std::unique_ptr<GameObject> extract_game_object(GameObject& game_object);
  bool remove_game_object(GameObject& game_object);

  [[nodiscard]] std::optional<std::reference_wrapper<Camera>> main_camera()
      const;
};
