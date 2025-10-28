#pragma once

#include <memory>

#include <engine/input/i_input_provider.h>

/**
 * @brief Centralized input manager that delegates to an external input provider.
 */
class InputManager {
public:
    IInputProvider* provider() const noexcept;
    void set_provider(std::unique_ptr<IInputProvider> provider) noexcept;

private:
    std::unique_ptr<IInputProvider> _provider = nullptr;
};
