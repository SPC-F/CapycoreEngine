#pragma once

#include <memory>
#include <engine/public/scene.h>
#include <engine/core/iEngineService.h>

class SceneService final : public IEngineService{
private:
    std::unique_ptr<Scene> scene_;
public:
    SceneService();
    SceneService(const SceneService&) = delete;
    SceneService& operator=(const SceneService&) = delete;

    SceneService(std::unique_ptr<Scene> initial_scene);
    ~SceneService() override;

    SceneService& load_scene(std::unique_ptr<Scene> scene);
    [[nodiscard]] Scene& current_scene() const;
};
