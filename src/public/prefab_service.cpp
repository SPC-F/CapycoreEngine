#include <engine/public/prefab_service.h>
#include <engine/public/scene.h>

#include <stdexcept>

void PrefabService::register_prefab(const std::string& prefab_type_id,
                                    const PrefabFactory& factory) {
  prefab_factories_[prefab_type_id] = factory;
}

void PrefabService::unregister_prefab(const std::string& prefab_type_id) {
  prefab_factories_.erase(prefab_type_id);
}

bool PrefabService::has_prefab(const std::string& prefab_type_id) const {
  return prefab_factories_.find(prefab_type_id) != prefab_factories_.end();
}

std::reference_wrapper<GameObject> PrefabService::instantiate(
    const std::string& prefab_type_id, Scene& scene, const std::string& name) {
  auto it = prefab_factories_.find(prefab_type_id);
  if (it == prefab_factories_.end()) {
    throw std::runtime_error("PrefabService: Unknown prefab type: " +
                             prefab_type_id);
  }

  return std::ref(it->second(scene, name));
}

std::vector<std::string> PrefabService::get_registered_prefabs() const {
  std::vector<std::string> result;
  result.reserve(prefab_factories_.size());
  for (const auto& pair : prefab_factories_) {
    result.push_back(pair.first);
  }
  return result;
}

void PrefabService::clear_all() { prefab_factories_.clear(); }
