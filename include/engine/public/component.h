#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <vector>

#include <cstddef>

namespace std { class type_info; }

class GameObject;

class Component {
 private:
  std::optional<std::reference_wrapper<GameObject>> parent_;
  bool active_{true};
  bool marked_for_deletion_{false};

  std::vector<std::function<void(Component&)>> on_detach_actions_;
  std::vector<std::function<void(Component&)>> on_attach_actions_;

 public:
  explicit Component();
  virtual ~Component() = default;

  [[nodiscard]] bool active() const noexcept;
  Component& active(bool value) noexcept;

  [[nodiscard]] bool marked_for_deletion() const noexcept;
  Component& mark_for_deletion() noexcept;

  virtual void update(float dt) = 0;

  virtual void on_attach();
  virtual void on_detach();

  // Legacy no-op serialize hooks (kept for compatibility)
  virtual void on_serialize();
  virtual void on_deserialize();

  // New payload-based hooks for network snapshots. Components that want to
  // include custom data in snapshots should override these. Default
  // implementations do nothing.
  virtual void on_serialize_payload(std::vector<uint8_t>& out) const;
  virtual void on_deserialize_payload(const std::vector<uint8_t>& data,
                                      size_t& offset);

  // A stable-ish textual identifier for the component type. By default this
  // returns the RTTI name; components may override to provide nicer names.
  virtual std::string type_name() const;

  const std::optional<std::reference_wrapper<GameObject>>& parent()
      const noexcept;  // NOLINT
  std::optional<std::reference_wrapper<GameObject>>&
  parent() noexcept;  // NOLINT
  Component& parent(GameObject& parent);
  Component& parent(std::nullopt_t nullopt);

  size_t add_on_attach(const std::function<void(Component&)>& action);
  void remove_on_attach(size_t index);

  size_t add_on_detach(const std::function<void(Component&)>& action);
  void remove_on_detach(size_t index);
};

template <typename T>
concept IsComponent = std::is_base_of_v<Component, T>;