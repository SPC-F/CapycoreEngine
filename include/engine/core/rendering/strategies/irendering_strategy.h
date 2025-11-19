#pragma once
#include <engine/core/rendering/renderer.h>

class IRenderingStrategy {
public:
    virtual ~IRenderingStrategy() = default;
    virtual void draw(Renderer& renderer) = 0;
};