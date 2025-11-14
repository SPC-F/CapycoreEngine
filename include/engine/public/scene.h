#pragma once

#include <vector>
#include <memory>

#include <engine/public/gameObject.h>

class Scene {
private:
    const std::string& name_;
    bool is_running_;
    float time_scale_;
    std::vector<std::unique_ptr<GameObject>> game_objects_;

    void game_loop();

public:
    Scene(const std::string& name);
    virtual ~Scene();

    void on_awake();
    void destroy();
    void run();
    void stop();

    Scene& time_scale(float modifier);
    [[nodiscard]] float time_scale() const;

    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] bool is_running() const;

    [[nodiscard]]
    std::reference_wrapper<GameObject> get_game_object(const std::string& name) const;
    [[nodiscard]]
    std::vector<std::reference_wrapper<GameObject>> game_objects() const;

    Scene& add_game_object(std::unique_ptr<GameObject> game_object);
    Scene& add_game_objects(std::vector<std::unique_ptr<GameObject>> game_objects);
    Scene& remove_game_object(const std::string& name);
};