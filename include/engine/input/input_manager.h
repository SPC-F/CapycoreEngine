#pragma once

#include <engine/core/iEngineService.h>
#include <engine/input/i_input_provider.h>

#include <memory>

/**
 * @brief Centralized input manager that delegates to an external input
 * provider.
 */
class InputManager : public IEngineService {
 public:
  [[nodiscard]] const IInputProvider& provider() const;
  void set_provider(std::unique_ptr<IInputProvider> provider) noexcept;
  void update();

 private:
  std::unique_ptr<IInputProvider> provider_ = nullptr;
};
