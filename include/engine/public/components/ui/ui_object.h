#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/util/point.h>

/**
 * @brief Base class for all UI objects.
 * 
 * UIObject represents a user interface element in the scene. It inherits from GameObject
 * and adds properties specific to UI elements, such as width, height, pivot, and anchor points.
 */
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

    /**
     * @brief Update the UI object.
     * 
     * This should be overridden by derived classes to implement specific update logic.
     * Such as handling animations or state changes (e.g pressed, hovered, etc.).
     * 
     * @param dt Delta time since the last update.
     */
    virtual void update(float dt) {};

    /**
     * @brief Render the UI object.
     * 
     * This should be overridden by derived classes to implement specific rendering logic.
     */
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