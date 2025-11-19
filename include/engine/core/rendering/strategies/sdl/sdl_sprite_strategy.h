#pragma once
#include <engine/core/rendering/strategies/irendering_strategy.h>

class SdlSpriteStrategy final : public IRenderingStrategy{
public:
    ~SdlSpriteStrategy() override = default;
    void draw(Renderer& renderer) override;
};
