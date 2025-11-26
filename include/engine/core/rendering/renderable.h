#pragma once

#include <memory>

#include <engine/core/rendering/strategies/irendering_strategy.h>
#include <engine/public/component.h>

/**
 * @brief Base class for renderable components in the engine.
 * 
 * Renderable components are those that can be drawn on the screen using a rendering strategy.
 * They hold a unique pointer to an IRenderingStrategy which defines how the component is rendered.
 * This class extends the Component class and provides functionality to set and retrieve the rendering strategy.
 */
class Renderable : public Component { // NOLINT
protected:
    std::unique_ptr<IRenderingStrategy> render_strategy_;
public:
    Renderable() = default;
    ~Renderable() override = default;

    void set_render_strategy(Component& component);
    [[nodiscard]] virtual IRenderingStrategy& render_strategy() const;
};