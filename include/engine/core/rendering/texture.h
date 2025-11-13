#pragma once
#include <memory>

class SDL_Texture;

class Texture final {
private:
    float width_;
    float height_;

    friend class Renderer;
    friend class RenderingManager;
    friend class AssetService;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture_; // custom deleter
    explicit Texture(SDL_Texture* texture);

public:
    [[nodiscard]] float width() const;

    [[nodiscard]] float height() const;
};
