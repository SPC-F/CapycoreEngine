#pragma once

#include <engine/public/component.h>
#include <engine/public/gameObject.h>

#include <functional>
#include <optional>
#include <stdexcept>

/**
 * @brief Base class for defining logic that can be attached to Components,
 *        similar to Unity's MonoBehaviour.
 *
 * Behaviors provide a structured lifecycle for game logic:
 * - on_awake()   : Called once when the Component is created.
 * - on_start()   : Called once before the first update.
 * - on_update()  : Called every frame.
 * - on_destroy() : Called when the Behavior or its GameObject is destroyed.
 *
 * A Behavior operates on the Component it is attached to and provides utility
 * methods for accessing the GameObject, Transform, and other Components. See it
 * as a gateway to implement game logic that interacts with the engine's
 * objects.
 *
 * @note Users should derive from this class to implement custom behavior.
 * @warning on_update() must be overridden.
 *
 * Usage:
 * - Create a class that inherits from Behavior.
 * - Override lifecycle methods (on_awake, on_start, on_update, on_destroy).
 * - Attach the Behavior to a Component via a BehaviorScript.
 * @see BehaviorScript
 */
class Behavior {
 public:
  Behavior();
  virtual ~Behavior() = default;

  /**
   * @brief Attaches this Behavior to a Component.
   * @param component The component this behavior should operate on.
   * @throws std::runtime_error if already attached.
   */
  void attach(Component& component);

  /// Called when the Component is created. Runs before on_start().
  virtual void on_awake(){};

  /// Called before the first frame on_update().
  virtual void on_start(){};

  /**
   * @brief Called once per frame.
   * @param dt Time delta since last frame.
   */
  virtual void on_update(float dt) = 0;

  /// Called when the Behavior or associated GameObject is destroyed.
  virtual void on_destroy(){};

  /**
   * @return Reference to the GameObject this Behavior operates on.
   * @throws std::runtime_error if not attached.
   */
  [[nodiscard]] GameObject& game_object() const;

  /**
   * @return Reference to the Transform of the attached GameObject.
   */
  [[nodiscard]] Transform& transform() const;

  /// @return Whether the behavior is currently enabled.
  [[nodiscard]] bool enabled() const;

  /// Enables the behavior.
  Behavior& enable();

  /// Disables the behavior.
  Behavior& disable();

  /**
   * @brief Serializes the behavior's state into a byte array.
   * @param out A vector to which the serialized bytes will be appended.
   * @return void
   */
  virtual void on_serialize(std::vector<uint8_t>& /*out*/) const {};

  /**
   * @brief Deserializes the behavior's state from a byte array.
   * @param data The byte array containing the serialized state.
   * @param offset The current offset in the byte array from which to start
   * reading. This offset will be updated as bytes are read.
   * @return void
   */
  virtual void on_deserialize(const std::vector<uint8_t>& /*data*/,
                              size_t& /*offset*/){};

  /**
   * @brief Retrieves a component of type T attached to the same GameObject.
   * @tparam T The type of the component to retrieve.
   * @return An optional reference to the component if found, std::nullopt
   * otherwise.
   * @throws std::runtime_error if not attached.
   */
  template <typename T>
  std::optional<std::reference_wrapper<T>> get_component() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    return game_object().template get_component<T>();
  }

  /**
   * @brief Retrieves all components of type T attached to the same GameObject.
   * @tparam T The type of the components to retrieve.
   * @return A vector of references to the components found.
   * @throws std::runtime_error if not attached.
   */
  template <typename T>
  std::vector<std::reference_wrapper<T>> get_components() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    return game_object().template get_components<T>();
  }

  /**
   * @brief Get a component of type T attached to this GameObject's children.
   * @tparam T The type of the component to get.
   * @return An optional reference to the component if found, std::nullopt
   * otherwise.
   */
  template <typename T>
  std::optional<std::reference_wrapper<T>> get_component_from_children() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    for (auto& child : game_object().children()) {
      auto comp = child.get().template get_component<T>();
      if (comp) {
        return comp;
      }
    }

    return std::nullopt;
  }

  /**
   * @brief Get all components of type T attached to this GameObject's children.
   * @tparam T The type of the component to get.
   * @return A vector of references to the components found.
   */
  template <typename T>
  std::vector<std::reference_wrapper<T>> get_components_from_children() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    return game_object().template get_components_from_children<T>();
  }

  /**
   * @brief Get a component of type T attached to this GameObject's parents.
   * @tparam T The type of the component to get.
   * @return An optional reference to the component if found, std::nullopt
   * otherwise.
   */
  template <typename T>
  std::optional<std::reference_wrapper<T>> get_component_in_parent() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    auto current_parent = game_object().parent();

    while (current_parent) {
      auto comp = current_parent->get().template get_component<T>();
      if (comp) {
        return comp;
      }
      current_parent = current_parent->get().parent();
    }

    return std::nullopt;
  }

  /**
   * @brief Get all components of type T attached to this GameObject's parents.
   * @tparam T The type of the component to get.
   * @return A vector of references to the components found.
   */
  template <typename T>
  std::vector<std::reference_wrapper<T>> get_components_in_parent() {
    if (!attached_component_) {
      throw std::runtime_error("Behavior has no associated GameObject.");
    }

    std::vector<std::reference_wrapper<T>> result;
    auto current_parent = game_object().parent();

    while (current_parent) {
      auto comps = current_parent->get().template get_components<T>();
      result.insert(result.end(), comps.begin(), comps.end());

      current_parent = current_parent->get().parent();
    }

    return result;
  }

  /// Destroy the parent game object
  void destroy();

  /// Destroy a specific component on the parent game object
  void destroy(Component& component);

  /// Destroy a specific game object in the scene
  void destroy(GameObject& game_object);

 private:
  std::optional<std::reference_wrapper<Component>> attached_component_;
  bool enabled_{true};
};