#include <engine/core/system/strategy/sdl_backend_system.h>
#include <engine/core/system/system_service.h>

SystemService::SystemService()
    : backend_system_(std::make_unique<SDLBackendSystem>()) {
  backend_system_->initialize_listeners(&listeners_);
}

int SystemService::add_listener(SystemEvent event, EventCallback callback) {
  listeners_[event].emplace_back(std::move(callback));
  return static_cast<int>(listeners_[event].size() - 1);
}

void SystemService::remove_listener(SystemEvent event, int listener_id) {
  auto& event_listeners = listeners_[event];
  if (listener_id >= 0 &&
      static_cast<size_t>(listener_id) < event_listeners.size()) {
    event_listeners.erase(event_listeners.begin() + listener_id);
  }
}

void SystemService::update() { backend_system_->poll_events(); }

std::string SystemService::get_clipboard_text() {
  return backend_system_->get_clipboard_text();
}

void SystemService::set_cursor_to_arrow() {
  backend_system_->set_cursor_to_arrow();
}

void SystemService::set_cursor_to_hand() {
  backend_system_->set_cursor_to_hand();
}

void SystemService::set_cursor_to_ibeam() {
  backend_system_->set_cursor_to_ibeam();
}

void SystemService::set_cursor_to_crosshair() {
  backend_system_->set_cursor_to_crosshair();
}

void SystemService::set_cursor_to_wait() {
  backend_system_->set_cursor_to_wait();
}

void SystemService::init_frame_timer() { backend_system_->init_frame_timer(); }

void SystemService::update_frame_time(float time_scale) {
  backend_system_->update_frame_time(time_scale);
}

float SystemService::delta_time() const {
  return backend_system_->delta_time();
}