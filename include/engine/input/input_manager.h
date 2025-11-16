#pragma once

#include <memory>

#include <engine/core/iEngineService.h>
#include <engine/input/i_input_provider.h>

/**
 * @brief Centralized input manager that delegates to an external input provider.
 */
class InputManager : public IEngineService {
public:
    [[nodiscard]] const IInputProvider& provider() const;
    void set_provider(std::unique_ptr<IInputProvider> provider) noexcept;

private:
    std::unique_ptr<IInputProvider> provider_ = nullptr;
};
