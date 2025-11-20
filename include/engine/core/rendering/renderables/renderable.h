#pragma once
#include <memory>
#include <engine/core/rendering/strategies/irendering_strategy.h>

class Renderable {
protected:
    std::unique_ptr<IRenderingStrategy> render_strategy_;
public:
    Renderable(std::unique_ptr<IRenderingStrategy> render_strategy);
    virtual ~Renderable() = default;
    [[nodiscard]] virtual IRenderingStrategy& render_strategy() const;
};