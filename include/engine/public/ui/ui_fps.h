#pragma once

#include <engine/public/ui/ui_object.h>

class UIFPS : public UIObject {
 public:
  explicit UIFPS(Scene& Scene);

  void update(float dt) override;
};