
#include <engine/core/rendering/irenderable.h>"

IRenderable::IRenderable(RenderStrategy *render_strategy) : render_strategy_(render_strategy) {

}

RenderStrategy& IRenderable::render_strategy() const {
    return *render_strategy_;
}
