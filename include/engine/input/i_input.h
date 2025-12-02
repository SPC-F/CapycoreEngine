#pragma once

#include <engine/input/key_code.h>
#include <engine/input/key_state.h>
#include <engine/input/mouse_state.h>

#include <map>

class IInput {
 public:
  virtual ~IInput() = default;

  /**
   * @brief Registers necessary event listeners with the underlying system.
   *
   * Must be called during initialization to ensure input events are captured.
   */
  virtual void register_events(std::map<KeyCode, KeyState>& key_states,
                               MouseState& mouse_state) = 0;
};
