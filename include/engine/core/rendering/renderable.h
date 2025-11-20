#pragma once
#include <memory>
#include "engine/core/rendering/strategies/irendering_strategy.h"
#include <engine/public/component.h>

class Renderable : public Component {
protected:
    std::unique_ptr<IRenderingStrategy> render_strategy_;
public:
    Renderable(std::unique_ptr<IRenderingStrategy> render_strategy);
    ~Renderable() override = default;
    [[nodiscard]] virtual IRenderingStrategy& render_strategy() const;
};