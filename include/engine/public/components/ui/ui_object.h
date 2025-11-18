#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/util/point.h>

class UIObject : public GameObject
{
public:
    UIObject(
        float width, 
        float height, 
        Point pivot, 
        Point anchor,
        Scene& scene
    );

    virtual void update(float dt) {};
    virtual void render() const = 0;

    [[nodiscard]] 
    float width() const;
    UIObject& width(float width);

    [[nodiscard]] 
    float height() const;
    UIObject& height(float height);

    [[nodiscard]] 
    Point pivot() const;
    UIObject& pivot(Point pivot);

    [[nodiscard]] 
    Point anchor() const;
    UIObject& anchor(Point anchor);

    [[nodiscard]]
    Scene& scene() const noexcept;
    UIObject& scene(Scene& scene);

protected:
    float width_;
    float height_;
    Point pivot_;
    Point anchor_;
};