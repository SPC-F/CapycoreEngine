#pragma once

#include <engine/public/ui/ui_object.h>

/**
 * @brief UI Component that displays the current frames per second (FPS).
 *
 * The UIFPS component shows the current FPS in the UI, allowing developers
 * to monitor performance in real-time.
 *
 * Usage:
 * - Create a UIFPS component and attach it to a UI GameObject.
 * - The component will automatically update and display the FPS during the
 *   update cycle.
 */
class UIFPS : public UIObject {
 public:
  explicit UIFPS(Scene& Scene);

  void update(float dt) override;
};