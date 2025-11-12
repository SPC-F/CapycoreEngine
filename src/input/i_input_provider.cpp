#include <engine/input/i_input_provider.h>

void IInputProvider::set_input(std::unique_ptr<IInput> input)
{
    input_ = std::move(input);
}

IInput& IInputProvider::input() const
{
    return *input_;
}
