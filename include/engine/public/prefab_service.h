#pragma once

#include <engine/core/iEngineService.h>
#include <engine/public/gameObject.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

class Scene;

/**
 * @brief A prefab factory function that creates a GameObject with predefined
 * components.
 *
 * The factory receives the scene and object name, and returns a fully
 * constructed GameObject with all its components already attached.
 */
using PrefabFactory = std::function<GameObject&(Scene&, const std::string&)>;

/**
 * @class PrefabService
 * @brief Global service for managing prefab factories and component creation.
 *
 * This is a singleton that maintains a mapping of prefab type IDs to factory
 * functions. When a network snapshot arrives with an object of an unknown type,
 * the service can instantiate it with the correct components.
 *
 * Usage:
 * - Register prefab factories with unique type IDs using register_prefab().
 * - Instantiate GameObjects from prefabs using instantiate().
 * - Check for registered prefabs with has_prefab().
 * - Unregister prefabs with unregister_prefab().
 * - Retrieve all registered prefab type IDs with get_registered_prefabs().
 * - Clear all registered prefabs with clear_all().
 * @note Prefab factories should create and return GameObjects with all
 * necessary components attached.
 */
class PrefabService : public IEngineService {
 public:
  PrefabService() = default;

  PrefabService(const PrefabService&) = delete;
  PrefabService& operator=(const PrefabService&) = delete;
  PrefabService(PrefabService&&) = delete;
  PrefabService& operator=(PrefabService&&) = delete;

  /**
   * @brief Register a prefab factory for a given type ID.
   * @param prefab_type_id Unique identifier for this prefab type
   * @param factory Factory function that creates the GameObject with components
   */
  void register_prefab(const std::string& prefab_type_id,
                       const PrefabFactory& factory);

  /**
   * @brief Unregister a previously registered prefab factory.
   * @param prefab_type_id Identifier of the prefab to unregister
   */
  void unregister_prefab(const std::string& prefab_type_id);

  /**
   * @brief Check if a prefab type is registered.
   * @param prefab_type_id Identifier to check
   * @return True if registered, false otherwise
   */
  [[nodiscard]] bool has_prefab(const std::string& prefab_type_id) const;

  /**
   * @brief Instantiate a GameObject from a registered prefab.
   * @param prefab_type_id Type ID of the prefab to instantiate
   * @param scene Scene to add the object to
   * @param name Name to assign to the created GameObject
   * @return Reference to the created GameObject
   * @throws std::runtime_error if prefab_type_id is not registered
   */
  std::reference_wrapper<GameObject> instantiate(
      const std::string& prefab_type_id, Scene& scene, const std::string& name);

  /**
   * @brief Get all registered prefab type IDs.
   */
  [[nodiscard]] std::vector<std::string> get_registered_prefabs() const;

  /**
   * @brief Clear all registered prefabs.
   */
  void clear_all();

 private:
  std::map<std::string, PrefabFactory> prefab_factories_;
};
