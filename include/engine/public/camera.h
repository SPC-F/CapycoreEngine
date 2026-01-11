#pragma once

#include <engine/public/gameObject.h>
#include <engine/public/util/color.h>

/**
 *  @brief Camera GameObject class
 * Inherits from GameObject class and represents a camera in the scene.
 * A scene requires at least one camera to render the view and can have multiple
 * cameras. However, only one camera can be designated as the main camera at a
 * time.
 *
 * Usage:
 * - Create a Camera object and attach it to a Scene.
 * - Configure properties such as background color, zoom level, and main camera
 *   status.
 */
class Camera final : public GameObject {
 private:
  Color backgroundColor_;
  float zoom_{1.0f};
  bool isMain_{false};

 public:
  explicit Camera(Scene& scene, Color backgroundColor, float zoom, bool isMain);

  /**
   * @brief Sets the background color of the camera.
   * @param color The Color object representing the desired background color.
   * This color will be used to clear the screen before rendering the scene.
   */
  void set_background_color(const Color& color);
  [[nodiscard]] Color background_color() const;

  /**
   * @brief Sets the zoom level of the camera.
   * @param zoom The desired zoom level. A zoom level of 1.0 represents
   * normal view, values greater than 1.0 zoom in, and values less than 1.0
   * zoom out.
   */
  void set_zoom(float zoom);
  [[nodiscard]] float zoom() const;

  /**
   * @brief The main camera is the one used for rendering the primary view of
   * the scene. Only one camera can be the main camera at any given time.
   */
  void set_main();
  void set_not_main();
  [[nodiscard]] bool is_main() const;

  /**
   * @brief Retrieves the current screen width in pixels.
   * @return The width of the screen in pixels as an integer.
   */
  [[nodiscard]] int get_screen_width() const;

  /**
   * @brief Retrieves the current screen height in pixels.
   * @return The height of the screen in pixels as an integer.
   */
  [[nodiscard]] int get_screen_height() const;
};