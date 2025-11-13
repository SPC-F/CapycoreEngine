#pragma once

#include <optional>

class SDL_Window; // forward declaration to keep SDL include out of headers

/**
 * @brief Represents the game window and provides methods to modify its properties.
 * The window encapsulates an SDL_Window, but it does not own it. The Window class is less of a wrapper and more of a controller. The actual owning is done by the Renderer.<br>
 * <br>
 * Note that the Window class provides explicit set_xyz methods. This is to avoid juggling with boolean parameters. You describe the desired state, you do not tell it how to get there.
 */
class Window {
private:
    unsigned min_width_;
    unsigned min_height_;

    // I tried using an std::optional<std::reference_wrapper<SDL_Window>> here. But optional requires a type know at compile time due to how the optional template is implemented.
    // SDL_Window * const can not be done here because init still has to be able to change its value.
    SDL_Window* window_;

    friend class Renderer;
    Window& init(SDL_Window* window);

public:
    explicit Window(unsigned min_width, unsigned min_height);

    Window& set_window_fullscreen();
    Window& set_window_windowed();
    Window& set_window_bordered();
    Window& set_window_borderless();
    Window& set_window_unresizable();
    Window& set_window_resizable();
    Window& set_window_width(unsigned width);
    Window& set_window_height(unsigned height);
};