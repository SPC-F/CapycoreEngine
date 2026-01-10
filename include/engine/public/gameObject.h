#pragma once

#include <engine/public/component.h>
#include <engine/public/transform.h>

#include <algorithm>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

class Scene;

/**
 * @brief Concept to constrain types to be derived from Component.
 * @tparam T The type to check.
 * GameObjects can have multiple components attached to them to define their
 * behavior and properties.
 *
 * Each GameObject has a unique identifier, a name, a tag, a layer, and a
 * transform representing its position, rotation, and scale in the scene.
 * GameObjects can also have parent-child relationships to form a hierarchy.
 *
 * GameObjects can be marked as active or inactive, which affects whether they
 * and their components are updated and rendered.
 *
 * GameObjects can be marked for deletion, which will remove them from the
 * scene at the end of the current update cycle.
 *
 * GameObjects can be marked as "don't destroy on load", which prevents them
 * from being destroyed when loading a new scene.
 */
class GameObject {
 private:
  std::string id_;

  std::vector<std::unique_ptr<Component>> components_;
  std::vector<std::reference_wrapper<GameObject>> children_;

  std::optional<std::reference_wrapper<GameObject>> parent_;
  bool is_active_{true};
  bool is_active_in_world_{true};

  std::string name_;
  std::string tag_;
  int layer_{};
  std::reference_wrapper<Scene> scene_;
  Transform transform_;

  bool marked_for_deletion_{false};
  bool dont_destroy_on_load_{false};
  std::string prefab_type_id_{};  // Identifier for prefab type used during
                                  // network replication

 public:
  explicit GameObject(Scene& scene);
  virtual ~GameObject() = default;

  GameObject(const GameObject&) = delete;
  GameObject& operator=(const GameObject&) = delete;

  GameObject(GameObject&&) = default;
  GameObject& operator=(GameObject&&) = default;

  std::optional<std::reference_wrapper<GameObject>> parent() const;  // NOLINT
  GameObject& parent(GameObject& parent);
  GameObject& parent(std::nullopt_t null_opt);

  [[nodiscard]] std::string id() const noexcept;

  /**
   * @brief Retrieves whether the GameObject is active in the world.
   * @return wether the GameObject is active in the world.
   */
  [[nodiscard]] bool is_active_in_world() const noexcept;

  /**
   * @brief Retrieves whether the GameObject is active.
   * @return wether the GameObject is active.
   */
  [[nodiscard]] bool is_active() const noexcept;

  void set_inactive() noexcept;
  void set_active() noexcept;
  void set_active_in_world() noexcept;
  void set_inactive_in_world() noexcept;

  /**
   * @brief Marks the GameObject for deletion.
   * @note The GameObject will be deleted at the end of the current scene update
   * cycle.
   *
   * @warning Once marked for deletion, all child GameObjects and components
   * will also be marked for deletion.
   *
   * @return wether the GameObject is marked for deletion.
   */
  GameObject& mark_for_deletion() noexcept;
  [[nodiscard]] bool marked_for_deletion() const noexcept;

  /**
   * @brief Sets whether the GameObject should not be destroyed on scene load.
   * @param dont_destroy true to prevent destruction on scene load, false to
   * @note only parent GameObjects can be marked as don't destroy on load.
   * allow destruction.
   * @return whether the GameObject is marked as don't destroy on load.
   */
  void mark_dont_destroy_on_load(bool dont_destroy) noexcept;
  bool dont_destroy_on_load() const noexcept;

  GameObject& name(const std::string& name);
  [[nodiscard]] const std::string& name() const;

  GameObject& tag(const std::string& tag);
  [[nodiscard]] const std::string& tag() const;

  GameObject& prefab_type_id(const std::string& id);
  [[nodiscard]] const std::string& prefab_type_id() const;

  GameObject& layer(int layer);
  [[nodiscard]] int layer() const;

  GameObject& transform(Transform transform);
  [[nodiscard]] Transform& transform();
  [[nodiscard]] const Transform& transform() const;

  [[nodiscard]] Scene& scene() const noexcept;
  void scene(Scene& scene) noexcept;

  [[nodiscard]] std::vector<std::reference_wrapper<GameObject>>& children();
  GameObject& add_child(GameObject& child);
  GameObject& remove_child(GameObject& child);

  /**
   * @brief Get the first component of type T attached to this GameObject.
   * @tparam T The type of the component to get. Must be derived from Component
   * @return An optional reference to the component if found, std::nullopt
   * otherwise.
   */
  template <IsComponent T>
  [[nodiscard]] std::optional<std::reference_wrapper<T>> get_component()
      const noexcept {
    for (const auto& component : components_) {
      if (auto* casted = dynamic_cast<T*>(component.get())) {
        return std::ref(*casted);
      }
    }
    return std::nullopt;
  }

  /**
   * @brief Get all components of type T attached to this GameObject.
   * @tparam T The type of the component to get. Must be derived from Component
   * @return A vector of references to the components found.
   */
  template <IsComponent T>
  [[nodiscard]] std::vector<std::reference_wrapper<T>> get_components() const {
    auto filtered =
        components_ |
        std::views::filter([](auto& c) { return dynamic_cast<T*>(c.get()); }) |
        std::views::transform([](auto& c) -> std::reference_wrapper<T> {
          return std::ref(*static_cast<T*>(c.get()));
        });

    return std::vector<std::reference_wrapper<T>>(filtered.begin(),
                                                  filtered.end());
  }

  /**
   * @brief Get the first script component of base type BS with behavior of type
   * B.
   * @tparam BS The base type of the script component. Must be derived from
   * Component
   * @tparam B The type of the behavior.
   * @return An optional reference to the behavior if found, std::nullopt
   * otherwise.
   */
  template <IsComponent BS, typename B>
  [[nodiscard]] std::optional<std::reference_wrapper<B>> get_script()
      const noexcept {
    auto scripts = get_components<BS>();

    for (const auto& script : scripts) {
      if (auto& behavior = script.get().behavior();
          dynamic_cast<B*>(&behavior)) {
        return std::ref(static_cast<B&>(behavior));
      }
    }

    return std::nullopt;
  }

  /**
   * @brief Get all components attached to this GameObject.
   * @return A vector of references to all components.
   */
  [[nodiscard]] std::vector<std::reference_wrapper<Component>>
  get_components_all() const;

  /**
   * @brief Get all components of type T attached to this GameObject and its
   * children.
   * @tparam T The type of the component to get. Must be derived from Component
   * @return A vector of references to the components found.
   */
  template <IsComponent T>
  std::vector<std::reference_wrapper<T>> get_components_from_children() const {
    std::vector<std::reference_wrapper<T>> result{};

    for (const auto& child : children_) {
      auto child_components = child.get().get_components_from_children<T>();
      result.insert(result.end(), child_components.begin(),
                    child_components.end());
    }

    return result;
  }

  /**
   * @brief Adds a component of type T to this GameObject.
   * @tparam T The type of the component to add. Must be derived from Component
   * @param args The arguments to forward to the component's constructor.
   * @return A reference to the added component.
   */
  template <IsComponent T, typename... Args>
  T& add_component(Args&&... args) {
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T& ref = *component;

    component->parent(std::ref(*this));
    component->on_attach();

    components_.emplace_back(std::move(component));
    return ref;
  }

  /**
   * @brief Removes a component of type T from this GameObject.
   * @tparam T The type of the component to remove. Must be derived from
   * Component
   * @param component The component to remove.
   * @CAUTION: This will call the component's on_detach method before removing
   * it.
   * @return void
   */
  template <IsComponent T>
  void remove_component(T& component) {
    component.on_detach();
    component.parent(std::nullopt);

    components_.erase(
        std::remove_if(components_.begin(), components_.end(),
                       [&component](const std::unique_ptr<Component>& c) {
                         return c.get() == &component;
                       }),
        components_.end());
  }

  void remove_all_components();

  /**
   * @brief Serialize this GameObject's own state and its components into `out`.
   * The format produced is:
   *   uint16_t name_len, name bytes
   *   uint16_t tag_len, tag bytes
   *   uint8_t is_active
   *   int32_t layer
   *   uint16_t component_count
   *   for each component:
   *     uint16_t type_name_len, type_name bytes
   *     uint32_t payload_len, payload bytes
   */
  void serialize(std::vector<uint8_t>& out) const;

  /**
   * @brief Deserialize this GameObject's state and dispatch component payloads
   * from `data` starting at `offset`. Implementations should advance `offset`
   * by the number of bytes consumed. Subclasses overriding this method should
   * call `GameObject::deserialize` first to keep base behavior.
   */
  void deserialize(const std::vector<uint8_t>& data, size_t& offset);
};
