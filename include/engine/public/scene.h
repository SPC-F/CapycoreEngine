#pragma once

#include <vector>
#include <memory>
#include <engine/public/gameObject.h>

class Scene {
private:
    bool is_running_;
    float time_modifier_;
    const std::vector<std::unique_ptr<GameObject>> game_objects_;

public:
    Scene(const std::string& name);
    virtual ~Scene();

    void on_awake();
    void destroy();
    void run();

    Scene& time_modifier(float modifier);
    float time_modifier() const;

    const std::vector<std::reference_wrapper<GameObject>> game_objects() const;
    Scene& add_game_object(std::unique_ptr<GameObject> game_object);
};