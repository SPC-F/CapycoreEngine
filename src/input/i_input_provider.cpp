#include <engine/input/i_input_provider.h>

void IInputProvider::set_input(std::unique_ptr<IInput> input)
{
    input_ = std::move(input);
}

std::optional<std::reference_wrapper<IInput>> IInputProvider::input() const
{
    if (input_) {
        return std::ref(*input_);
    }
    return std::nullopt;
}
