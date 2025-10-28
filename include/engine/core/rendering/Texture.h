#pragma once
#include <memory>

class SDL_Texture;
class Renderer;

class Texture final {
public:
    void width(float value);
    void height(float value);
    [[nodiscard]] float width() const;

    [[nodiscard]] float height() const;

private:
    float _width{0}, _height{0};

    friend class Renderer;
    friend class RenderingManager;
    friend class AssetManager;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> _texture; // custom deleter
    explicit Texture(SDL_Texture* texture);
};
