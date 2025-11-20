#pragma once
#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/components/sprite.h>
#include <SDL3/sdl.h>

class SdlSpriteStrategy final : public IRenderingStrategy{
private:
    Sprite& sprite_;
    GameObject& game_object_;
    SDL_Renderer* const sdl_renderer_;
public:
    SdlSpriteStrategy(Sprite& sprite, GameObject& parent, SDL_Renderer* renderer);
    ~SdlSpriteStrategy() override = default;
    void draw(Renderer& renderer) override;
};
