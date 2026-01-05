#include <engine/network/snapshot.h>
#include <engine/public/component.h>
#include <engine/public/gameObject.h>
#include <engine/public/scene.h>
#include <engine/util/uuid.h>

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

GameObject::GameObject(Scene& scene)
    : id_(uuid::generate_uuid_v4()), scene_(scene) {}

std::string GameObject::id() const noexcept { return id_; }

GameObject& GameObject::name(const std::string& name) {
  name_ = name;
  return *this;
}
const std::string& GameObject::name() const { return name_; }

GameObject& GameObject::tag(const std::string& tag) {
  tag_ = tag;
  return *this;
}
const std::string& GameObject::tag() const { return tag_; }

GameObject& GameObject::prefab_type_id(const std::string& id) {
  prefab_type_id_ = id;
  return *this;
}
const std::string& GameObject::prefab_type_id() const {
  return prefab_type_id_;
}

GameObject& GameObject::layer(const int layer) {
  layer_ = layer;
  return *this;
}
int GameObject::layer() const { return layer_; }

GameObject& GameObject::transform(Transform transform) {
  transform_ = transform;
  return *this;
}
Transform& GameObject::transform() { return transform_; }

const Transform& GameObject::transform() const { return transform_; }

Scene& GameObject::scene() const noexcept { return scene_; }

void GameObject::scene(Scene& scene) noexcept { scene_ = scene; }

void GameObject::set_inactive() noexcept { is_active_ = false; }
void GameObject::set_active() noexcept { is_active_ = true; }
void GameObject::set_active_in_world() noexcept { is_active_in_world_ = true; }
void GameObject::set_inactive_in_world() noexcept {
  is_active_in_world_ = false;
}

bool GameObject::is_active_in_world() const noexcept {
  if (parent_.has_value()) {
    return is_active_in_world_ && parent_->get().is_active_in_world();
  }

  return is_active_in_world_;
}

bool GameObject::is_active() const noexcept {
  if (parent_.has_value()) {
    return is_active_ && parent_->get().is_active();
  }

  return is_active_;
}

bool GameObject::marked_for_deletion() const noexcept {
  return marked_for_deletion_;
}

GameObject& GameObject::mark_for_deletion() noexcept {
  marked_for_deletion_ = true;
  return *this;
}

void GameObject::mark_dont_destroy_on_load(const bool dont_destroy) noexcept {
  if (!parent().has_value()) {
    dont_destroy_on_load_ = dont_destroy;
  }
}

bool GameObject::dont_destroy_on_load() const noexcept {
  if (dont_destroy_on_load_) return true;

  if (parent().has_value()) {
    return parent_->get().dont_destroy_on_load();
  }

  return false;
}
std::optional<std::reference_wrapper<GameObject>> GameObject::parent() const {
  return parent_;
}

GameObject& GameObject::parent(GameObject& parent) {
  parent_ = parent;

  const auto found_child =
      std::find_if(parent.children().begin(), parent.children().end(),
                   [&](auto& ref) { return &ref.get() == this; });

  if (found_child == parent.children().end()) {
    parent.add_child(*this);
  }

  transform_.parent(std::ref(parent.transform()));

  return *this;
}

GameObject& GameObject::parent(std::nullopt_t null_opt) {
  if (!parent_.has_value()) {
    return *this;
  }

  parent_ = std::nullopt;
  return *this;
}

std::vector<std::reference_wrapper<GameObject>>& GameObject::children() {
  return children_;
}

std::vector<std::reference_wrapper<Component>> GameObject::get_components_all()
    const {
  std::vector<std::reference_wrapper<Component>> result;
  result.reserve(components_.size());
  for (const auto& c : components_) {
    result.emplace_back(*c);
  }
  return result;
}

void GameObject::remove_all_components() {
  for (auto& component : components_) {
    component->on_detach();
    component->parent(std::nullopt);
  }
  components_.clear();
}

GameObject& GameObject::add_child(GameObject& child) {
  children_.emplace_back(child);
  child.parent(*this);
  child.transform().parent(std::ref(this->transform()));
  return *this;
}

GameObject& GameObject::remove_child(GameObject& child) {
  std::erase_if(children_, [&](auto& ref) { return &ref.get() == &child; });
  child.transform().parent(std::nullopt);
  child.parent(std::nullopt);
  return *this;
}

void GameObject::serialize(std::vector<uint8_t>& out) const {
  // Write object metadata
  snapshot::write_string(out, prefab_type_id_);
  snapshot::write_string(out, name_);
  snapshot::write_string(out, tag_);

  uint8_t active = is_active_ ? 1 : 0;
  snapshot::write_bytes(out, &active, sizeof(active));

  int32_t layer = static_cast<int32_t>(layer_);
  snapshot::write_bytes(out, &layer, sizeof(layer));

  // Write transform: position (3 floats), rotation (1 float), scale (3 floats)
  auto pos = transform_.position();
  float rot = transform_.rotation();
  auto sc = transform_.scale();
  snapshot::write_bytes(out, &pos.x, sizeof(float));
  snapshot::write_bytes(out, &pos.y, sizeof(float));
  snapshot::write_bytes(out, &pos.z, sizeof(float));
  snapshot::write_bytes(out, &rot, sizeof(float));
  snapshot::write_bytes(out, &sc.x, sizeof(float));
  snapshot::write_bytes(out, &sc.y, sizeof(float));
  snapshot::write_bytes(out, &sc.z, sizeof(float));

  // Serialize components
  auto comps = get_components_all();
  std::vector<std::pair<std::string, std::vector<uint8_t>>> comp_entries;
  for (auto& c : comps) {
    std::vector<uint8_t> cp;
    c.get().on_serialize(cp);
    if (!cp.empty())
      comp_entries.emplace_back(c.get().type_name(), std::move(cp));
  }

  uint16_t comp_count = static_cast<uint16_t>(comp_entries.size());
  snapshot::write_bytes(out, &comp_count, sizeof(comp_count));

  for (const auto& e : comp_entries) {
    snapshot::write_string(out, e.first);
    uint32_t payload_length = static_cast<uint32_t>(e.second.size());
    snapshot::write_bytes(out, &payload_length, sizeof(payload_length));
    if (payload_length > 0) {
      snapshot::write_bytes(out, e.second.data(), payload_length);
    }
  }
}

void GameObject::deserialize(const std::vector<uint8_t>& data, size_t& offset) {
  // Read object metadata
  if (!snapshot::read_string(data, offset, prefab_type_id_)) return;
  if (!snapshot::read_string(data, offset, name_)) return;
  if (!snapshot::read_string(data, offset, tag_)) return;

  uint8_t active = 0;
  if (!snapshot::read_bytes(data, offset, &active, sizeof(active))) return;
  is_active_ = (active != 0);

  int32_t layer = 0;
  if (!snapshot::read_bytes(data, offset, &layer, sizeof(layer))) return;
  layer_ = static_cast<int>(layer);

  // Read transform: position (3 floats), rotation (1 float), scale (3 floats)
  float px = 0.0f, py = 0.0f, pz = 0.0f;
  if (!snapshot::read_bytes(data, offset, &px, sizeof(float))) return;
  if (!snapshot::read_bytes(data, offset, &py, sizeof(float))) return;
  if (!snapshot::read_bytes(data, offset, &pz, sizeof(float))) return;

  float rot = 0.0f;
  if (!snapshot::read_bytes(data, offset, &rot, sizeof(float))) return;

  float sx = 1.0f, sy = 1.0f, sz = 1.0f;
  if (!snapshot::read_bytes(data, offset, &sx, sizeof(float))) return;
  if (!snapshot::read_bytes(data, offset, &sy, sizeof(float))) return;
  if (!snapshot::read_bytes(data, offset, &sz, sizeof(float))) return;

  transform_.position({px, py, pz});
  transform_.rotation(rot);
  transform_.scale({sx, sy, sz});

  // Deserialize components
  uint16_t comp_count = 0;
  if (!snapshot::read_bytes(data, offset, &comp_count, sizeof(comp_count)))
    return;

  for (uint16_t i = 0; i < comp_count; ++i) {
    std::string type_name;
    if (!snapshot::read_string(data, offset, type_name)) break;

    uint32_t payload_length = 0;
    if (!snapshot::read_bytes(data, offset, &payload_length,
                              sizeof(payload_length)))
      break;

    // Dispatch payload to matching component
    bool applied = false;
    for (auto& comp_ref : get_components_all()) {
      if (comp_ref.get().type_name() == type_name) {
        size_t inner_off = offset;
        comp_ref.get().on_deserialize(data, inner_off);
        applied = true;
        break;
      }
    }

    // Advance offset past this component's payload regardless
    offset += payload_length;
  }
}
// NOLINTEND