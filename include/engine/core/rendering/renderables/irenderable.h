#pragma once
#include <engine/core/rendering/strategies/irendering_strategy.h>

class IRenderable {
private:
    IRenderingStrategy * const render_strategy_;
public:
    explicit IRenderable(IRenderingStrategy * render_strategy);
    virtual ~IRenderable() = default;

    IRenderingStrategy& render_strategy() const;
};