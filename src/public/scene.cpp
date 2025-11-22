#include <engine/public/scene.h>

#include <SDL3/SDL.h>
#include <algorithm>

#include <engine/util/memory.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/engine.h>
#include <engine/input/input_manager.h>
#include <engine/input/i_input_provider.h>

constexpr float accumulator_default_value = 0.0f;
constexpr float fixed_step = 1.0f / 60.0f; // ~60 fps

Scene::Scene(const std::string& name) // NOLINT
    : name_{ name },
      is_running_{ false },
      time_scale_{ 1.0f } {}

Scene::~Scene() {
    execute_listeners(destroy_listeners_);
}

void Scene::on_run(listener_function_t& listener) {
    run_listeners_.push_back(listener);
}

void Scene::on_stop(listener_function_t& listener) {
    stop_listeners_.push_back(listener);
}

void Scene::on_destroy(listener_function_t& listener) {
    destroy_listeners_.push_back(listener);
}

void Scene::execute_listeners(const std::vector<Scene::listener_function_t> &listeners) {
    for (const auto& listener : listeners) {
        listener(*this);
    }
}

void Scene::game_loop() { // NOLINT [readability-make-member-function-const]
    float accumulator = accumulator_default_value;

    auto& rendering_service = Engine::instance().services->get_service<RenderingService>().get();
    rendering_service.init_frame_timer();

    while (is_running()) {
        rendering_service.update_frame_time(time_scale_);
        float frame_dt = rendering_service.delta_time();

        accumulator += frame_dt;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                stop();
            }
        }

        while (accumulator >= fixed_step) {
            // creates a fixed step for input handling and physics updates
            accumulator -= fixed_step;
        }

        // So tracy logs all allocations, even the past ones in previous frames
        // It does this to build a complete timeline of allocations for profiling
        // We don't want that overhead during normal frame rendering as clearing is buggy here due to the stack
        // So we run the rendering without tracy tracking (if tracy is enabled)
        run_without_tracy([&]() {
            auto game_objects = this->game_objects();
            rendering_service.draw(game_objects);
        });
    }
}

void Scene::run() {
    is_running_ = true;
    execute_listeners(run_listeners_);
    game_loop();
}

void Scene::stop() {
    is_running_ = false;
    execute_listeners(stop_listeners_);
}

Scene& Scene::time_scale(float modifier) {
    time_scale_ = modifier;
    return *this;
}
float Scene::time_scale() const {
    return time_scale_;
}

const std::string& Scene::name() const {
    return name_;
}

bool Scene::is_running() const {
    return is_running_;
}

std::reference_wrapper<GameObject> Scene::get_game_object(const std::string& id) const {
    for(const auto& game_object : game_objects_) {
        if (game_object->id() == id) {
            return *game_object;
        }
    }

    throw std::runtime_error("GameObject with id " + id + " not found in scene " + name_);
}

std::vector<std::reference_wrapper<GameObject>> Scene::game_objects() const {
    std::vector<std::reference_wrapper<GameObject>> refs;
    refs.reserve(game_objects_.size());
    for (const auto& game_object : game_objects_) {
        refs.emplace_back(*game_object);
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

    return *object_ptr; // could be done with a stored ref as well, but this makes it clearer what is returned
}

Scene& Scene::add_game_objects(std::vector<std::unique_ptr<GameObject>> game_objects) {
    for (auto& game_object : game_objects) {
        game_objects_.emplace_back(std::move(game_object));
    }
    return *this;
}

bool Scene::remove_game_object(GameObject& game_object) {
    auto found_object = std::find_if(
        game_objects_.begin(),
        game_objects_.end(),
        [&game_object](const auto& param) {
            return param.get() == &game_object;
        });

    if (found_object == game_objects_.end()) {
        return false; // not found
    }

    game_objects_.erase(found_object);

    return true;
}