#include <engine/core/system/strategy/sdl_backend_system.h>

void SDLBackendSystem::initialize_listeners(
    std::unordered_map<SystemEvent, std::vector<BackendCallback>>* listeners) {
  listeners_ = listeners;
}

void SDLBackendSystem::poll_events() {
  SDL_Event e;

  auto execute_callbacks = [this](SystemEvent event_type, SDL_Event& event) {
    for (auto& cb : (*listeners_)[event_type]) {
      cb(&event);
    }
  };

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        execute_callbacks(EVENT_QUIT, e);
        break;
      case SDL_EVENT_KEY_DOWN:
        execute_callbacks(KEY_DOWN, e);
        break;
      case SDL_EVENT_KEY_UP:
        execute_callbacks(KEY_UP, e);
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        execute_callbacks(MOUSE_BUTTON_DOWN, e);
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        execute_callbacks(MOUSE_BUTTON_UP, e);
        break;
      case SDL_EVENT_MOUSE_MOTION:
        execute_callbacks(MOUSE_MOTION, e);
        break;
      case SDL_EVENT_MOUSE_WHEEL:
        execute_callbacks(MOUSE_WHEEL, e);
        break;

      default:
        break;
    }
  }
}

void SDLBackendSystem::init_frame_timer() {
  last_ = SDL_GetPerformanceCounter();
  freq_ = static_cast<double>(SDL_GetPerformanceFrequency());

  frame_count_ = 0;
  fps_ = 0.0f;
  accumulated_time_ = 0.0;
}

void SDLBackendSystem::update_frame_time(float time_scale) {
  Uint64 now = SDL_GetPerformanceCounter();
  double raw_delta = static_cast<double>(now - last_) / freq_;
  last_ = now;

  delta_time_ = static_cast<float>(raw_delta * time_scale);

  accumulated_time_ += raw_delta;
  frame_count_++;

  if (accumulated_time_ >= 1.0) {
    fps_ = static_cast<float>(frame_count_ / accumulated_time_);
    frame_count_ = 0;
    accumulated_time_ = 0.0;
  }
}

float SDLBackendSystem::delta_time() const { return delta_time_; }

float SDLBackendSystem::frames_per_second() const { return fps_; }

std::string SDLBackendSystem::get_clipboard_text() {
  char* text = SDL_GetClipboardText();

  if (text) {
    std::string clipboard_text(text);
    SDL_free(text);  // NOLINT
    return clipboard_text;
  }

  return "";
}

void SDLBackendSystem::set_cursor_to_arrow() {
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT));
}

void SDLBackendSystem::set_cursor_to_hand() {
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER));
}

void SDLBackendSystem::set_cursor_to_ibeam() {
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT));
}

void SDLBackendSystem::set_cursor_to_crosshair() {
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
}

void SDLBackendSystem::set_cursor_to_wait() {
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT));
}