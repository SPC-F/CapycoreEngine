#include <engine/core/engine.h>
#include <engine/core/rendering/renderingService.h>
#include <engine/core/rendering/window.h>
#include <engine/public/camera.h>
#include <engine/public/scene.h>

Camera::Camera(Scene& scene, const Color backgroundColor, const float zoom,
               const bool isMain)
    : GameObject(scene),
      backgroundColor_(backgroundColor),
      zoom_(zoom),
      isMain_(isMain) {}

void Camera::set_background_color(const Color& color) {
  backgroundColor_ = color;
}

Color Camera::background_color() const { return backgroundColor_; }

void Camera::set_zoom(const float zoom) { zoom_ = zoom; }

float Camera::zoom() const { return zoom_; }

void Camera::set_main() {
  isMain_ = true;
  auto game_objects = scene().game_objects();
  for (auto& obj_ref : game_objects) {
    GameObject& obj = obj_ref.get();
    if (!dynamic_cast<Camera*>(&obj)) {
      continue;
    }

    auto& cam = dynamic_cast<Camera&>(obj);
    if (&cam != this) {
      cam.set_not_main();
    }
  }
}

void Camera::set_not_main() { isMain_ = false; }

bool Camera::is_main() const { return isMain_; }

int Camera::get_screen_width() const {
  auto& window = Engine::instance()
                     .services->get_service<RenderingService>()
                     .get()
                     .window();

  return window.get_window_width();
}

int Camera::get_screen_height() const {
  auto& window = Engine::instance()
                     .services->get_service<RenderingService>()
                     .get()
                     .window();

  return window.get_window_height();
}