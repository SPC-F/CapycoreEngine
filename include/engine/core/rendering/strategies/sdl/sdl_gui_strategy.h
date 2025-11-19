#pragma once
#include <engine/core/rendering/strategies/irendering_strategy.h>

class SdlGuiStrategy final : public IRenderingStrategy {
public:
    ~SdlGuiStrategy() override = default;
    void draw(Renderer& renderer) override;
};