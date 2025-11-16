#include <engine/public/scene.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/engine.h>
#include <SDL3/SDL.h>

Scene::Scene(const std::string& name)
    : is_running_(false), time_scale_(1.0f), game_objects_ {}, name_{name} {
}

Scene::~Scene() {
    destroy();
}

void Scene::on_awake() {

}

void Scene::destroy() {

}

void Scene::game_loop() {
    float accumulator = 0.0f;
    const float fixed_step = 1.0f / 60.0f; // ~60 fps

    Uint64 last = SDL_GetPerformanceCounter();
    float freq = static_cast<float>(SDL_GetPerformanceFrequency());

    auto& rendering_service = Engine::instance()
            .services
            ->get_service<RenderingService>()
            .get();

    while (true) {
        Uint64 now = SDL_GetPerformanceCounter();
        float frame_dt = static_cast<float>(now - last) / freq * time_scale_;
        last = now;

        accumulator += frame_dt;

        while (accumulator >= fixed_step) {
            //physics(fixed_step, 8, 3);
            accumulator -= fixed_step;
        }

        rendering_service.draw(game_objects());
    }

}

void Scene::run() {
    is_running_ = true;
    game_loop();
}

void Scene::stop() {
    is_running_ = false;
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

std::reference_wrapper<GameObject> Scene::get_game_object(const std::string& name) const {
    game_objects_
    | std::views::filter([&name](const auto& game_object) {
          return game_object->name() == name;
      })
    | std::views::take(1)
    | std::views::transform([](const auto& game_object) -> std::reference_wrapper<GameObject> {
          return *game_object;
    });

    throw std::runtime_error("GameObject with name " + name + " not found in scene " + name_);
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
    game_object->name(name);
    game_objects_.emplace_back(std::move(game_object));

    return *game_object.get(); // could be done with a stored ref as well, but this makes it clearer what is returned
}

Scene& Scene::add_game_objects(std::vector<std::unique_ptr<GameObject>> game_objects) {
    for (auto& game_object : game_objects) {
        game_objects_.emplace_back(std::move(game_object));
    }
    return *this;
}

Scene& Scene::remove_game_object(const std::string& name) {
    auto it = std::remove_if(
        game_objects_.begin(),
        game_objects_.end(),
         [&name](const auto& game_object) {
             return game_object->name() == name;
         });

    if (it != game_objects_.end()) {
        game_objects_.erase(it, game_objects_.end());
    }

    return *this;
}