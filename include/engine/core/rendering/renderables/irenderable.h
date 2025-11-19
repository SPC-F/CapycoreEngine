#pragma once
#include <memory>
#include <engine/core/rendering/strategies/irendering_strategy.h>

class IRenderable {
protected:
    std::unique_ptr<IRenderingStrategy> render_strategy_;
public:
    virtual ~IRenderable() = default;
    [[nodiscard]] virtual IRenderingStrategy& render_strategy() const = 0;
};