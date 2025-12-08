#include <engine/public/component.h>
#include <engine/public/gameObject.h>
#include <engine/public/scene.h>
#include <engine/util/uuid.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdint>

GameObject::GameObject(Scene& scene)
    : id_(uuid::generate_uuid_v4()), scene_(scene) {}

GameObject::~GameObject() {
  if (parent_.has_value()) {
    parent_->get().remove_child(*this);
  }

  for (auto child : children_) {
    scene_.get().remove_game_object(child);
  }
}

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

const Scene& GameObject::scene() const noexcept { return scene_; }

void GameObject::scene(Scene& scene) noexcept { scene_ = scene; }

void GameObject::set_inactive() noexcept { is_active_ = false; }
void GameObject::set_active() noexcept { is_active_ = true; }
void GameObject::set_active_in_world() noexcept { is_active_in_world_ = true; }
void GameObject::set_inactive_in_world() noexcept {
  is_active_in_world_ = false;
}

bool GameObject::is_active_in_world() const noexcept {
  return is_active_in_world_;
}

bool GameObject::is_active() const noexcept { return is_active_; }

bool GameObject::marked_for_deletion() const noexcept {
  return marked_for_deletion_;
}

GameObject& GameObject::mark_for_deletion() noexcept {
  marked_for_deletion_ = true;
  return *this;
}

void GameObject::mark_dont_destroy_on_load(const bool destroy) noexcept {
  if (!parent().has_value()) {
    dont_destroy_on_load_ = destroy;
  }
}

bool GameObject::dont_destroy_on_load() const noexcept {
  return dont_destroy_on_load_;
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

std::vector<std::reference_wrapper<Component>> GameObject::get_components_all() const {
  std::vector<std::reference_wrapper<Component>> result;
  result.reserve(components_.size());
  for (const auto& c : components_) {
    result.emplace_back(*c);
  }
  return result;
}

GameObject& GameObject::add_child(GameObject& child) {
  children_.emplace_back(child);
  child.parent(*this);
  return *this;
}

GameObject& GameObject::remove_child(GameObject& child) {
  std::erase_if(children_, [&](auto& ref) { return &ref.get() == &child; });
  child.parent(std::nullopt);
  return *this;
}

// NOLINTBEGIN
void GameObject::serialize(std::vector<uint8_t>& out) const {
  // write name
  uint16_t name_len = static_cast<uint16_t>(name_.size());
  size_t old = out.size();
  out.resize(old + sizeof(name_len) + name_len);
  uint8_t* write_ptr = out.data() + old;
  std::memcpy(write_ptr, &name_len, sizeof(name_len)); write_ptr += sizeof(name_len);
  if (name_len > 0) { std::memcpy(write_ptr, name_.data(), name_len); write_ptr += name_len; }

  // write tag
  uint16_t tag_len = static_cast<uint16_t>(tag_.size());
  size_t after_tag = write_ptr - out.data();
  out.resize(out.size() + sizeof(tag_len) + tag_len);
  write_ptr = out.data() + after_tag;
  std::memcpy(write_ptr, &tag_len, sizeof(tag_len)); write_ptr += sizeof(tag_len);
  if (tag_len > 0) { std::memcpy(write_ptr, tag_.data(), tag_len); write_ptr += tag_len; }

  // is_active
  uint8_t active = is_active_ ? 1 : 0;
  size_t after_active = write_ptr - out.data();
  out.resize(out.size() + sizeof(active));
  write_ptr = out.data() + after_active;
  std::memcpy(write_ptr, &active, sizeof(active)); write_ptr += sizeof(active);

  // layer (int32_t)
  int32_t layer = static_cast<int32_t>(layer_);
  size_t after_layer = write_ptr - out.data();
  out.resize(out.size() + sizeof(layer));
  write_ptr = out.data() + after_layer;
  std::memcpy(write_ptr, &layer, sizeof(layer)); write_ptr += sizeof(layer);

  // transform: local position (3 floats), rotation (1 float), scale (3 floats)
  auto lp = transform_.local_position();
  float rot = transform_.rotation();
  auto sc = transform_.scale();
  size_t after_transform = write_ptr - out.data();
  out.resize(out.size() + sizeof(float) * 7);
  write_ptr = out.data() + after_transform;
  std::memcpy(write_ptr, &lp.x, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &lp.y, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &lp.z, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &rot, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &sc.x, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &sc.y, sizeof(float)); write_ptr += sizeof(float);
  std::memcpy(write_ptr, &sc.z, sizeof(float)); write_ptr += sizeof(float);

  // components
  auto comps = get_components_all();
  std::vector<std::pair<std::string, std::vector<uint8_t>>> comp_entries;
  for (auto& c : comps) {
    std::vector<uint8_t> cp;
    c.get().on_serialize(cp);
    if (!cp.empty()) comp_entries.emplace_back(c.get().type_name(), std::move(cp));
  }

  uint16_t comp_count = static_cast<uint16_t>(comp_entries.size());
  size_t after_compcount = write_ptr - out.data();
  out.resize(out.size() + sizeof(comp_count));
  write_ptr = out.data() + after_compcount;
  std::memcpy(write_ptr, &comp_count, sizeof(comp_count)); write_ptr += sizeof(comp_count);

  // write components
  for (const auto& e : comp_entries) {
    uint16_t tlen = static_cast<uint16_t>(e.first.size());
    uint32_t plen = static_cast<uint32_t>(e.second.size());
    size_t cur_off = write_ptr - out.data();
    out.resize(out.size() + sizeof(tlen) + tlen + sizeof(plen) + plen);
    write_ptr = out.data() + cur_off;
    std::memcpy(write_ptr, &tlen, sizeof(tlen)); write_ptr += sizeof(tlen);
    if (tlen > 0) { std::memcpy(write_ptr, e.first.data(), tlen); write_ptr += tlen; }
    std::memcpy(write_ptr, &plen, sizeof(plen)); write_ptr += sizeof(plen);
    if (plen > 0) { std::memcpy(write_ptr, e.second.data(), plen); write_ptr += plen; }
  }
}

void GameObject::deserialize(const std::vector<uint8_t>& data, size_t& offset) {
  // read name
  if (offset + sizeof(uint16_t) > data.size()) return;
  uint16_t name_len = 0;
  std::memcpy(&name_len, data.data() + offset, sizeof(name_len)); offset += sizeof(name_len);
  if (offset + name_len > data.size()) return;
  name_.assign(reinterpret_cast<const char*>(data.data() + offset), name_len); offset += name_len;

  // read tag
  if (offset + sizeof(uint16_t) > data.size()) return;
  uint16_t tag_len = 0;
  std::memcpy(&tag_len, data.data() + offset, sizeof(tag_len)); offset += sizeof(tag_len);
  if (offset + tag_len > data.size()) return;
  tag_.assign(reinterpret_cast<const char*>(data.data() + offset), tag_len); offset += tag_len;

  // is_active
  if (offset + sizeof(uint8_t) > data.size()) return;
  uint8_t active = 0; std::memcpy(&active, data.data() + offset, sizeof(active)); offset += sizeof(active);
  is_active_ = (active != 0);

  // layer
  if (offset + sizeof(int32_t) > data.size()) return;
  int32_t layer = 0; std::memcpy(&layer, data.data() + offset, sizeof(layer)); offset += sizeof(layer);
  layer_ = static_cast<int>(layer);

  // transform: local position (3 floats), rotation (1 float), scale (3 floats)
  if (offset + sizeof(float) * 7 > data.size()) return;
  float px = 0.0f, py = 0.0f, pz = 0.0f;
  std::memcpy(&px, data.data() + offset, sizeof(float)); offset += sizeof(float);
  std::memcpy(&py, data.data() + offset, sizeof(float)); offset += sizeof(float);
  std::memcpy(&pz, data.data() + offset, sizeof(float)); offset += sizeof(float);
  float rot = 0.0f; std::memcpy(&rot, data.data() + offset, sizeof(float)); offset += sizeof(float);
  float sx = 1.0f, sy = 1.0f, sz = 1.0f;
  std::memcpy(&sx, data.data() + offset, sizeof(float)); offset += sizeof(float);
  std::memcpy(&sy, data.data() + offset, sizeof(float)); offset += sizeof(float);
  std::memcpy(&sz, data.data() + offset, sizeof(float)); offset += sizeof(float);
  transform_.position({px, py, pz});
  transform_.rotation(rot);
  transform_.scale({sx, sy, sz});

  // components
  if (offset + sizeof(uint16_t) > data.size()) return;
  uint16_t comp_count = 0; std::memcpy(&comp_count, data.data() + offset, sizeof(comp_count)); offset += sizeof(comp_count);

  for (uint16_t i = 0; i < comp_count; ++i) {
    if (offset + sizeof(uint16_t) > data.size()) { offset = data.size(); break; }
    uint16_t tlen = 0; std::memcpy(&tlen, data.data() + offset, sizeof(tlen)); offset += sizeof(tlen);
    if (offset + tlen > data.size()) { offset = data.size(); break; }
    std::string tname;
    if (tlen > 0) { tname.assign(reinterpret_cast<const char*>(data.data() + offset), tlen); offset += tlen; }

    if (offset + sizeof(uint32_t) > data.size()) { offset = data.size(); break; }
    uint32_t plen = 0; std::memcpy(&plen, data.data() + offset, sizeof(plen)); offset += sizeof(plen);
    if (offset + plen > data.size()) { offset = data.size(); break; }

    // dispatch payload to matching component if present
    bool applied = false;
    for (auto& comp_ref : get_components_all()) {
      if (comp_ref.get().type_name() == tname) {
        size_t inner_off = offset;
        comp_ref.get().on_deserialize(data, inner_off);
        // advance offset to end of this component payload regardless
        offset = offset + plen;
        applied = true;
        break;
      }
    }
    if (!applied) {
      // skip unknown component payload
      offset += plen;
    }
  }
}
// NOLINTEND