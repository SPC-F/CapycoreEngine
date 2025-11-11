#pragma once

#include <optional>

class SDL_Window; // forward declaration to keep SDL include out of headers

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