#pragma once
#include <memory>

class SDL_Texture; // NOLINT

/**
 * @brief Represents a texture used in rendering game objects.
 * Encapsulates an SDL_Texture and provides access to its dimensions.
 */
class Texture final {
private:
    float width_;
    float height_;

    friend class Renderer;
    friend class RenderingManager;
    friend class AssetService;
    friend class SdlSpriteStrategy;

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture_; // custom deleter
    explicit Texture(SDL_Texture* texture);

public:
    [[nodiscard]] float width() const;
    [[nodiscard]] float height() const;
};
