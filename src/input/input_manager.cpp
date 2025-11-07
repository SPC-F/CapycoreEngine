#include <engine/input/input_manager.h>
#include <engine/input/i_input_provider.h>

const IInputProvider& InputManager::provider() const
{
    if (!provider_)
        throw std::runtime_error("Input provider is not set.");
    return *provider_;
}

void InputManager::set_provider(std::unique_ptr<IInputProvider> provider) noexcept
{
    provider_ = std::move(provider);
}
