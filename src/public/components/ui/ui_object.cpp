#include <engine/public/components/ui/ui_object.h>

UIObject::UIObject(
    float width, 
    float height, 
    Point pivot, 
    Point anchor,
    Scene& scene
) : 
    GameObject(scene), 
    width_(width), 
    height_(height), 
    pivot_(pivot), 
    anchor_(anchor)
{}

float UIObject::width() const {
    return width_;
}

UIObject& UIObject::width(float width) {
    width_ = width;
    return *this;
}

float UIObject::height() const {
    return height_;
}

UIObject& UIObject::height(float height) {
    height_ = height;
    return *this;
}

Point UIObject::pivot() const {
    return pivot_;
}

UIObject& UIObject::pivot(Point pivot) {
    pivot_ = pivot;
    return *this;
}

Point UIObject::anchor() const {
    return anchor_;
}

UIObject& UIObject::anchor(Point anchor) {
    anchor_ = anchor;
    return *this;
}