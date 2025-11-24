#include <engine/public/camera.h>

Camera::Camera(
    Scene& scene,
    const Color backgroundColor,
    const float aspectWidth,
    const float aspectHeight,
    const float zoom,
    const bool isMain) : GameObject(scene),
        backgroundColor_(backgroundColor),
        aspectWidth_(aspectWidth),
        aspectHeight_(aspectHeight),
        zoom_(zoom),
        isMain_(isMain) {
}

void Camera::set_background_color(const Color& color) {
    backgroundColor_ = color;
}

Color Camera::background_color() const {
    return backgroundColor_;
}

void Camera::set_aspect_width(const float width) {
    aspectWidth_ = width;
}

float Camera::aspect_width() const {
    return aspectWidth_;
}

void Camera::set_aspect_height(const float height) {
    aspectHeight_ = height;
}

float Camera::aspect_height() const {
    return aspectHeight_;
}

void Camera::set_zoom(const float zoom) {
    zoom_ = zoom;
}

float Camera::zoom() const {
    return zoom_;
}

void Camera::set_main() {
    isMain_ = true;
}

void Camera::set_not_main() {
    isMain_ = false;
}

bool Camera::is_main() const {
    return isMain_;
}