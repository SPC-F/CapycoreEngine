#pragma once
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <vector>

class GameObject;

/**
 * @brief Base class for all components that can be attached to GameObjects.
 * @warning All derived components must implement the type_name() method to
 * provide a consistent and portable type identifier for serialization purposes.
 *
 * This is crucial for ensuring that component types can be accurately
 * identified across different compilers and platforms, as relying on
 * typeid(T).name() can lead to inconsistencies.
 *
 * @note this class should not be used directly. Instead, create derived
 * component classes that inherit from Component.
 */
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

  /**
   * @brief Checks if the component is active.
   * @return whether the component is active.
   */
  [[nodiscard]] bool active() const noexcept;
  Component& active(bool value) noexcept;

  /**
   * @brief Activates the component.
   * @return A reference to this Component for chaining.
   */
  [[nodiscard]] bool marked_for_deletion() const noexcept;
  Component& mark_for_deletion() noexcept;

  virtual void update(float dt) = 0;

  /** @brief Called when the component is attached to a GameObject. */
  virtual void on_attach();
  /** @brief Called when the component is detached from a GameObject. */
  virtual void on_detach();

  /**
   * @brief Serializes the component's state into a byte array.
   * @param out A vector to which the serialized bytes will be appended.
   * @return A vector of bytes representing the serialized state of the
   * component.
   */
  virtual void on_serialize(std::vector<uint8_t>& /*out*/) const;

  /**
   * @brief Deserializes the component's state from a byte array.
   * @param data The byte array containing the serialized state.
   * @param offset The current offset in the byte array from which to start
   * reading. This offset will be updated as bytes are read.
   * @return void
   */
  virtual void on_deserialize(const std::vector<uint8_t>& /*data*/,
                              size_t& /*offset*/);

  /**
   * @brief Provides a consistent type name for the component.
   * @return A string representing the type name of the component.
   * @CAUTION: All derived components must implement this method to ensure
   * consistent serialization across different compilers and platforms.
   */
  virtual std::string type_name() const = 0;

  /**
   * @brief Gets the parent GameObject of this component.
   * @return An optional reference to the parent GameObject, if it exists.
   */
  const std::optional<std::reference_wrapper<GameObject>>& parent()
      const noexcept;  // NOLINT

  /**
   * @brief Gets the parent GameObject of this component.
   * @param parent The GameObject to set as the parent.
   * @return An optional reference to the parent GameObject, if it exists.
   */
  std::optional<std::reference_wrapper<GameObject>>&
  parent() noexcept;  // NOLINT

  Component& parent(GameObject& parent);
  Component& parent(std::nullopt_t nullopt);

  /**
   * @brief Retrieves the type information of the component.
   * @param action The action to be performed on attach.
   * @return A reference to the std::type_info object representing the
   * component's type.
   */
  size_t add_on_attach(const std::function<void(Component&)>& action);
  void remove_on_attach(size_t index);

  /**
   * @brief Adds an action to be performed when the component is attached.
   * @param action The action to be performed on attach.
   * @return the index of the added action, which can be used to remove it
   * later.
   */
  size_t add_on_detach(const std::function<void(Component&)>& action);
  void remove_on_detach(size_t index);
};

template <typename T>
concept IsComponent = std::is_base_of_v<Component, T>;