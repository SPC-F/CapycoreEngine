#pragma once

#include <engine/input/i_input_provider.h>

/**
 * @brief Centralized input manager that delegates to an external input provider.
 */
class InputManager {
public:
    IInputProvider* provider() const noexcept;
    void set_provider(IInputProvider* provider) noexcept;

private:
    IInputProvider* _provider = nullptr;
};
