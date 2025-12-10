#include <engine/network/snapshot.h>
#include <engine/public/components/sprite.h>
#include <engine/public/components/network_identity.h>
#include <engine/public/util/vector3.h>
#include <engine/public/prefab_registry.h>

#include <cstring>
#include <string>
#include <cstdint>
#include <typeinfo>

namespace snapshot {

static constexpr size_t UUID_LEN = 37;

// Helper: Write a length-prefixed string to the output buffer
void write_string(std::vector<uint8_t>& out, const std::string& str) {
  uint16_t len = static_cast<uint16_t>(str.size());
  size_t old = out.size();
  out.resize(old + sizeof(len) + len);
  uint8_t* ptr = out.data() + old;
  std::memcpy(ptr, &len, sizeof(len));
  if (len > 0) {
    std::memcpy(ptr + sizeof(len), str.data(), len);
  }
}

// Helper: Read a length-prefixed string from the input buffer
bool read_string(const std::vector<uint8_t>& data, size_t& offset, std::string& out) {
  if (offset + sizeof(uint16_t) > data.size()) return false;
  uint16_t len = 0;
  std::memcpy(&len, data.data() + offset, sizeof(len));
  offset += sizeof(len);
  if (offset + len > data.size()) return false;
  out.assign(reinterpret_cast<const char*>(data.data() + offset), len);
  offset += len;
  return true;
}

// Helper: Write raw bytes to the output buffer
void write_bytes(std::vector<uint8_t>& out, const void* data, size_t size) {
  size_t old = out.size();
  out.resize(old + size);
  std::memcpy(out.data() + old, data, size);
}

// Helper: Read raw bytes from the input buffer
bool read_bytes(const std::vector<uint8_t>& data, size_t& offset, void* out, size_t size) {
  if (offset + size > data.size()) return false;
  std::memcpy(out, data.data() + offset, size);
  offset += size;
  return true;
}

// Helper: Serialize a single networked GameObject
static void serialize_object(const GameObject& go, const NetworkIdentity& net, std::vector<uint8_t>& payload)
{
    char uuidbuf[UUID_LEN] = {};
    std::strncpy(uuidbuf, net.uuid().c_str(), UUID_LEN - 1);

    std::vector<uint8_t> obj_payload;
    go.serialize(obj_payload);

    size_t entry_size = UUID_LEN + obj_payload.size();
    size_t old = payload.size();
    payload.resize(old + entry_size);
    uint8_t* write_ptr = payload.data() + old;
    std::memcpy(write_ptr, uuidbuf, UUID_LEN);
    write_ptr += UUID_LEN;
    if (!obj_payload.empty()) {
        std::memcpy(write_ptr, obj_payload.data(), obj_payload.size());
    }
}

// Helper: Create snapshot message with a filter predicate
template<typename FilterFunc>
static Message create_snapshot(const Scene& scene, DefaultMessageTypes messageType, FilterFunc&& filter)
{
    Message msg;
    msg.header.type = MessageType(messageType);

    auto objects = scene.game_objects();

    // Count and serialize objects matching the filter
    uint16_t count = 0;
    std::vector<uint8_t> payload;
    payload.reserve(sizeof(count) + objects.size() * (UUID_LEN + sizeof(uint16_t) * 2));
    payload.resize(sizeof(count));

    for (const auto& go_ref : objects) {
        const auto& go = go_ref.get();
        auto net_opt = go.get_component<NetworkIdentity>();
        if (!net_opt) continue;

        auto& net = net_opt->get();
        if (filter(net)) {
            serialize_object(go, net, payload);
            ++count;
        }
    }

    // Write count at the beginning
    std::memcpy(payload.data(), &count, sizeof(count));

    msg.payload = std::move(payload);
    msg.header.size = static_cast<uint32_t>(msg.payload.size());
    return msg;
}

// Helper: Find or create GameObject by UUID
static GameObject* find_or_create_object(Scene& scene, const std::string& uuid_str, 
                                         const std::vector<uint8_t>& payload, size_t offset)
{
    // Find existing object
    for (auto& go_ref : scene.game_objects()) {
        auto comp_opt = go_ref.get().get_component<NetworkIdentity>();
        if (comp_opt && comp_opt->get().uuid() == uuid_str) {
            return &go_ref.get();
        }
    }

    // Create new object from prefab
    if (offset + sizeof(uint16_t) > payload.size())
        return nullptr;

    size_t peek_off = offset;
    uint16_t prefab_id_len = 0;
    std::memcpy(&prefab_id_len, payload.data() + peek_off, sizeof(prefab_id_len));
    peek_off += sizeof(prefab_id_len);

    if (peek_off + prefab_id_len > payload.size())
        return nullptr;

    std::string prefab_type_id(reinterpret_cast<const char*>(
        payload.data() + peek_off), prefab_id_len);

    auto& registry = PrefabRegistry::instance();
    if (!registry.has_prefab(prefab_type_id))
        return nullptr;

    try {
        auto* obj = &registry.instantiate(prefab_type_id, scene, uuid_str);
        if (!obj->get_component<NetworkIdentity>()) {
            obj->add_component<NetworkIdentity>(uuid_str);
        }
        return obj;
    } catch (const std::exception&) {
        return nullptr;
    }
}

Message create_full_snapshot(const Scene& scene, DefaultMessageTypes messageType)
{
    return create_snapshot(scene, messageType, [](const NetworkIdentity&) { return true; });
}

Message create_delta_snapshot(const Scene& scene, DefaultMessageTypes messageType)
{
    auto msg = create_snapshot(scene, messageType, [](const NetworkIdentity& net) {
        return net.is_dirty();
    });

    // Clear dirty flags after snapshotting
    for (auto& go_ref : scene.game_objects()) {
        auto net_opt = go_ref.get().get_component<NetworkIdentity>();
        if (net_opt && net_opt->get().is_dirty()) {
            net_opt->get().clear_dirty();
        }
    }

    return msg;
}

// Advances the offset over a serialized GameObject payload without constructing one.
static void skip_gameobject_payload(const std::vector<uint8_t>& data, size_t& offset)
{
    auto skip_string = [&data, &offset]() {
        if (offset + sizeof(uint16_t) > data.size()) return false;
        uint16_t len = 0;
        std::memcpy(&len, data.data() + offset, sizeof(len));
        offset += sizeof(len);
        if (offset + len > data.size()) { offset = data.size(); return false; }
        offset += len;
        return true;
    };

    auto skip_bytes = [&data, &offset](size_t count) {
        if (offset + count > data.size()) { offset = data.size(); return false; }
        offset += count;
        return true;
    };

    // Skip object metadata
    if (!skip_string()) return;           // prefab_type_id
    if (!skip_string()) return;           // name
    if (!skip_string()) return;           // tag
    if (!skip_bytes(sizeof(uint8_t))) return;   // is_active
    if (!skip_bytes(sizeof(int32_t))) return;   // layer
    if (!skip_bytes(sizeof(float) * 7)) return; // transform (7 floats)

    // Skip components
    if (offset + sizeof(uint16_t) > data.size()) return;
    uint16_t comp_count = 0;
    std::memcpy(&comp_count, data.data() + offset, sizeof(comp_count));
    offset += sizeof(comp_count);

    for (uint16_t i = 0; i < comp_count; ++i) {
        if (!skip_string()) break;  // component type name

        if (offset + sizeof(uint32_t) > data.size()) break;
        uint32_t plen = 0;
        std::memcpy(&plen, data.data() + offset, sizeof(plen));
        offset += sizeof(plen);

        if (!skip_bytes(plen)) break; // component payload
    }
}

void apply_full_snapshot(Scene& scene, const Message& msg)
{
    apply_delta_snapshot(scene, msg);
}

void apply_delta_snapshot(Scene& scene, const Message& msg)
{
    const auto& payload = msg.payload;
    if (payload.size() < sizeof(uint16_t))
        return;

    size_t offset = 0;
    uint16_t count = 0;
    std::memcpy(&count, payload.data() + offset, sizeof(count));
    offset += sizeof(count);

    for (uint16_t i = 0; i < count; ++i) {
        if (offset + UUID_LEN + sizeof(uint16_t) > payload.size())
            break;

        char uuidbuf[UUID_LEN] = {};
        std::memcpy(uuidbuf, payload.data() + offset, UUID_LEN);
        offset += UUID_LEN;

        GameObject* obj = find_or_create_object(scene, std::string(uuidbuf), payload, offset);

        if (obj) {
            obj->deserialize(payload, offset);
        } else {
            skip_gameobject_payload(payload, offset);
        }
    }
}

} // namespace snapshot
