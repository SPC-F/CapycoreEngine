#include <engine/core/rendering/window.h>
#include <SDL3/SDL.h>

Window::Window(unsigned min_width, unsigned min_height)
    : min_width_(min_width), min_height_(min_height), window_(nullptr) {}

Window& Window::init(SDL_Window* window) {
    window_ = window;
    return *this;
}

Window& Window::set_window_fullscreen() {
    SDL_SetWindowFullscreen(window_, true);
    return *this;
}
Window& Window::set_window_windowed() {
    SDL_SetWindowFullscreen(window_, false);
    return *this;
}
Window& Window::set_window_borderless() {
    SDL_SetWindowBordered(window_, true);
    return *this;
}
Window& Window::set_window_bordered() {
    SDL_SetWindowBordered(window_, false);
    return *this;
}
Window& Window::set_window_resizable() {
    SDL_SetWindowResizable(window_, true);
    return *this;
}
Window& Window::set_window_unresizable() {
    SDL_SetWindowResizable(window_, false);
    return *this;
}

Window& Window::set_window_width(unsigned width) {
    int current_height {};
    SDL_GetWindowSize(window_, nullptr, &current_height);
    SDL_SetWindowSize(window_, static_cast<int>(width), current_height);
    return *this;
}
Window& Window::set_window_height(unsigned height) {
    int current_width {};
    SDL_GetWindowSize(window_, &current_width, nullptr);
    SDL_SetWindowSize(window_, current_width, static_cast<int>(height));
    return *this;
}
