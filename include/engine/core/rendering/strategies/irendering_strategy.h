#pragma once

#include <engine/core/rendering/renderer.h>

/**
 * @brief Interface for rendering strategies used by renderable components.
 * 
 * IRenderingStrategy defines a contract for rendering strategies that can be implemented
 * to provide different rendering techniques. Each strategy must implement the draw method
 * which takes a Component reference and performs the rendering logic.
 */
class IRenderingStrategy {
public:
    virtual ~IRenderingStrategy() = default;
    virtual void draw(Component& component) = 0;
};