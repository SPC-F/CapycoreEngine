#include <SDL3/SDL.h>
#include <engine/audio/audio_service.h>
#include <engine/core/engine.h>
#include <engine/core/rendering/renderable.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/system/system_service.h>
#include <engine/input/input_manager.h>
#include <engine/input/input_system.h>
#include <engine/network/multiplayer_service.h>
#include <engine/physics/physics_service.h>
#include <engine/public/component.h>
#include <engine/public/gameObject.h>
#include <engine/public/gameplay_speed_service.h>
#include <engine/public/scene.h>
#include <engine/public/ui/ui_object.h>
#include <engine/util/memory.h>

#include <algorithm>

constexpr float accumulator_default_value = 0.0f;
constexpr float fixed_step = 1.0f / 60.0f;  // ~60 fps

Scene::Scene(const std::string& name)  // NOLINT
    : name_{name}, is_running_{false}, is_stopping_{false}, time_scale_{1.0f} {}

Scene::~Scene() { execute_listeners(destroy_listeners_); }

void Scene::on_run(listener_function_t&& listener) {
  run_listeners_.push_back(listener);
}

void Scene::on_stop(listener_function_t&& listener) {
  stop_listeners_.push_back(listener);
}

void Scene::on_destroy(listener_function_t&& listener) {
  destroy_listeners_.push_back(listener);
}

void Scene::execute_listeners(
    const std::vector<Scene::listener_function_t>& listeners) {
  for (const auto& listener : listeners) {
    listener(*this);
  }
}

void Scene::game_loop() {  // NOLINT [readability-make-member-function-const]
  if (!is_running()) {
    return;
  }

  float accumulator = accumulator_default_value;

  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  auto& audio_service =
      Engine::instance().services->get_service<AudioService>().get();
  auto& input_manager =
      Engine::instance().services->get_service<InputManager>().get();
  auto& physics_service =
      Engine::instance().services->get_service<PhysicsService>().get();
  auto& rendering_service =
      Engine::instance().services->get_service<RenderingService>().get();
  auto& multiplayer_service =
      Engine::instance().services->get_service<MultiplayerService>().get();
  auto& gameplay_speed_service =
      Engine::instance().services->get_service<GameplaySpeedService>().get();

  system_service.init_frame_timer();

  while (is_running()) {
    // 1. update frame time
    system_service.update_frame_time(time_scale_ *
                                     gameplay_speed_service.speed());
    float frame_dt = system_service.delta_time();
    accumulator += frame_dt;

    // 2. handle input & system events
    run_without_tracy([&]() {
      input_manager.update();
      system_service.update();

      multiplayer_service.poll();
    });

    std::map<int, std::multimap<int, std::reference_wrapper<Renderable>>>
        layered_renderables{};
    auto game_objects = this->active_game_objects();

    // 3. fixed update for physics and other fixed-timestep systems
    while (accumulator >= fixed_step) {
      run_without_tracy([this, &game_objects, &physics_service]() {
        physics_service.update(fixed_step, game_objects);
      });

      accumulator -= fixed_step;
    }

    std::vector<std::reference_wrapper<GameObject>> marked_for_deletion{};

    // 4. update game objects & components, collect renderables
    // So tracy logs all allocations, even the past ones in previous frames
    // It does this to build a complete timeline of allocations for profiling
    // We don't want that overhead during normal frame rendering as clearing is
    // buggy here due to the stack So we run the rendering without tracy
    // tracking (if tracy is enabled)
    run_without_tracy([&]() {
      audio_service.update();
      gameplay_speed_service.update();

      for (auto& game_object_ref : game_objects) {
        auto& game_object = game_object_ref.get();
        if (!game_object.is_active_in_world() || !game_object.is_active()) {
          continue;
        }

        // add game-object layer
        if (!layered_renderables.contains(game_object.layer())) {
          layered_renderables.try_emplace(game_object.layer());
        }

        auto& obj_layer = layered_renderables.at(game_object.layer());

        if (auto* ui_object_opt = dynamic_cast<UIObject*>(&game_object)) {
          ui_object_opt->update(frame_dt);
        }

        auto components = game_object.get_components<Component>();

        for (auto& component_ref : components) {
          auto& component = component_ref.get();
          if (!component.active()) {
            continue;
          }

          component.update(frame_dt);

          if (auto* const renderable = dynamic_cast<Renderable*>(&component)) {
            obj_layer.emplace(renderable->order_in_layer(), *renderable);
          }
        }

        if (game_object.marked_for_deletion()) {
          marked_for_deletion.push_back(game_object);
        }

        if (is_stopping_ || !is_running_) break;
      }

      rendering_service.draw(layered_renderables, *this);

      // 5. cleanup marked for deletion game objects AFTER rendering
      cleanup_destroyed_game_objects();
    });

    // 6. check for stop condition
    if (marked_for_stopping()) {
      is_running_ = false;
    }
  }
}

void Scene::run() {
  is_running_ = true;
  execute_listeners(run_listeners_);

  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  stop_event_listener_id_ = system_service.add_listener(
      EVENT_QUIT, [&](void* /*event*/) {  // register per scene
        stop();
      });

  game_loop();
}

void Scene::stop() {
  is_running_ = false;
  execute_listeners(stop_listeners_);

  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  system_service.remove_listener(EVENT_QUIT, stop_event_listener_id_);
}

bool Scene::marked_for_stopping() const { return is_stopping_; }

void Scene::mark_for_stopping() noexcept { is_stopping_ = true; }

Scene& Scene::time_scale(float modifier) {
  time_scale_ = modifier;
  return *this;
}
float Scene::time_scale() const { return time_scale_; }

const std::string& Scene::name() const { return name_; }

bool Scene::is_running() const { return is_running_; }

std::reference_wrapper<GameObject> Scene::get_game_object(
    const std::string& id) const {
  for (const auto& game_object : game_objects_) {
    if (game_object->id() == id) {
      return *game_object;
    }
  }

  throw std::runtime_error("GameObject with id " + id + " not found in scene " +
                           name_);
}

std::vector<std::reference_wrapper<GameObject>> Scene::game_objects() const {
  std::vector<std::reference_wrapper<GameObject>> refs;
  refs.reserve(game_objects_.size());
  for (const auto& game_object : game_objects_) {
    refs.emplace_back(*game_object);
  }
  return refs;
}

std::vector<std::reference_wrapper<GameObject>> Scene::active_game_objects()
    const {
  std::vector<std::reference_wrapper<GameObject>> refs;
  refs.reserve(game_objects_.size());
  for (const auto& game_object : game_objects_) {
    auto& game_object_ref = *game_object;
    if (game_object_ref.is_active() && game_object_ref.is_active_in_world()) {
      refs.emplace_back(*game_object);
    }
  }
  return refs;
}

Scene& Scene::add_game_object(std::unique_ptr<GameObject> game_object) {
  game_objects_.emplace_back(std::move(game_object));
  return *this;
}

GameObject& Scene::add_game_object(const std::string& name) {
  auto game_object = std::make_unique<GameObject>(*this);
  auto* object_ptr = game_object.get();
  game_object->name(name);
  game_objects_.emplace_back(std::move(game_object));

  return *object_ptr;  // could be done with a stored ref as well, but this
                       // makes it clearer what is returned
}

Scene& Scene::add_game_objects(
    std::vector<std::unique_ptr<GameObject>> game_objects) {
  for (auto& game_object : game_objects) {
    game_objects_.emplace_back(std::move(game_object));
  }
  return *this;
}

/**
 * @brief Extracts and transfers ownership of a game object from the scene.
 *
 * Extracts a specified game object from the scene, transferring its ownership.
 * Removes the game object from the scene's internal collection if found.
 *
 * @return A unique pointer to the extracted game object if it is found and
 * successfully removed from the scene; otherwise, returns nullptr.
 */
std::unique_ptr<GameObject> Scene::extract_game_object(
    GameObject& game_object) {
  const auto found_object =
      std::ranges::find_if(game_objects_, [&game_object](const auto& param) {
        return param.get() == &game_object;
      });

  if (found_object == game_objects_.end()) {
    return nullptr;
  }

  std::unique_ptr<GameObject> extracted = std::move(*found_object);
  game_objects_.erase(found_object);
  return extracted;
}

bool Scene::remove_game_object(GameObject& game_object) {
  auto found_object = std::find_if(
      game_objects_.begin(), game_objects_.end(),
      [&game_object](const auto& obj) { return obj.get() == &game_object; });

  if (found_object == game_objects_.end()) {
    return false;
  }

  found_object->get()->mark_for_deletion();

  return true;
}

void Scene::cleanup_destroyed_game_objects() {
  /// Recursively mark and detach all children and components of the game object
  /// for deletion. This ensures that when a parent game object is deleted,
  /// all its children and their components are also properly marked and
  /// detached. We can't do this in loop as we would modify the children vector
  /// while iterating it.
  auto mark_detach_recursive = [](GameObject& obj, auto& self) -> void {
    std::vector<std::reference_wrapper<GameObject>> children_to_process;
    for (auto& child_ref : obj.children()) {
      children_to_process.push_back(child_ref.get());
    }

    for (auto& child : children_to_process) {
      self(child.get(), self);        // Recurse first
      obj.remove_child(child.get());  // Then detach
      child.get().mark_for_deletion();
    }
  };

  for (auto& obj : game_objects_) {
    if (obj->marked_for_deletion()) {
      mark_detach_recursive(*obj, mark_detach_recursive);
      obj->remove_all_components();
    }
  }

  std::erase_if(game_objects_, [](const std::unique_ptr<GameObject>& obj) {
    return obj->marked_for_deletion();
  });
}

std::optional<std::reference_wrapper<Camera>> Scene::main_camera() const {
  for (const auto& game_object : game_objects_) {
    if (!dynamic_cast<Camera*>(game_object.get())) {
      continue;
    }

    auto& camera = dynamic_cast<Camera&>(*game_object);

    if (!camera.is_main()) {
      continue;
    }

    return camera;
  }

  return std::nullopt;
}