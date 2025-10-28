#pragma once
#include <memory>

class SDL_Texture;
class Renderer;

class Texture final {
private:
    float width_{0}, height_{0};

    friend class Renderer;
    friend class RenderingManager;
    friend class AssetManager;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture_; // custom deleter
    explicit Texture(SDL_Texture* texture);

public:
    [[nodiscard]] float width() const;

    [[nodiscard]] float height() const;
};
