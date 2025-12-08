#include <engine/network/snapshot.h>
#include <engine/public/components/sprite.h>
#include <engine/public/components/network_identity.h>
#include <engine/public/util/vector3.h>

#include <cstring>
#include <string>
#include <cstdint>
#include <typeinfo>

namespace snapshot {

static constexpr size_t UUID_LEN = 37;

Message create_full_snapshot(const Scene& scene, DefaultMessageTypes messageType)
{
    Message msg;
    msg.header.type = MessageType(messageType);

    auto objects = scene.game_objects();

    // Count networked objects
    uint16_t count = 0;
    for (const auto& go_ref : objects) {
        const auto& go = go_ref.get();
        if (go.get_component<NetworkIdentity>())
            ++count;
    }

    std::vector<uint8_t> payload;
    payload.reserve(sizeof(count) + count * (UUID_LEN + sizeof(uint16_t) * 2));

    // write count
    payload.resize(sizeof(count));
    std::memcpy(payload.data(), &count, sizeof(count));

    for (const auto& go_ref : objects) {
        const auto& go = go_ref.get();
        auto net_opt = go.get_component<NetworkIdentity>();
        if (!net_opt)
            continue;

        const auto& net = net_opt->get();
        char uuidbuf[UUID_LEN] = {};
        std::strncpy(uuidbuf, net.uuid().c_str(), UUID_LEN - 1);

        // Let the GameObject produce its full per-object payload
        std::vector<uint8_t> obj_payload;
        go.serialize(obj_payload);

        size_t entry_size = UUID_LEN + obj_payload.size();
        size_t old = payload.size();
        payload.resize(old + entry_size);
        uint8_t* write_ptr = payload.data() + old;
        std::memcpy(write_ptr, uuidbuf, UUID_LEN); write_ptr += UUID_LEN;
        if (!obj_payload.empty()) {
            std::memcpy(write_ptr, obj_payload.data(), obj_payload.size());
        }
    }

    msg.payload = std::move(payload);
    msg.header.size = static_cast<uint32_t>(msg.payload.size());
    return msg;
}

Message create_delta_snapshot(const Scene& scene, DefaultMessageTypes messageType)
{
    Message msg;
    msg.header.type = MessageType(messageType);

    auto objects = scene.game_objects();

    // Count dirty networked objects
    uint16_t count = 0;
    for (const auto& go_ref : objects) {
        const auto& go = go_ref.get();
        auto net_opt = go.get_component<NetworkIdentity>();
        if (net_opt && net_opt->get().is_dirty())
            ++count;
    }

    std::vector<uint8_t> payload;
    payload.reserve(sizeof(count) + count * (UUID_LEN + sizeof(uint16_t) * 2));
    payload.resize(sizeof(count));
    std::memcpy(payload.data(), &count, sizeof(count));

    for (const auto& go_ref : objects) {
        const auto& go = go_ref.get();
        auto net_opt = go.get_component<NetworkIdentity>();
        if (!net_opt)
            continue;

        auto& net = net_opt->get();
        if (!net.is_dirty())
            continue;

        char uuidbuf[UUID_LEN] = {};
        std::strncpy(uuidbuf, net.uuid().c_str(), UUID_LEN - 1);

        // Let the GameObject produce its full per-object payload
        std::vector<uint8_t> obj_payload;
        go.serialize(obj_payload);

        size_t entry_size = UUID_LEN + obj_payload.size();
        size_t old = payload.size();
        payload.resize(old + entry_size);
        uint8_t* write_ptr = payload.data() + old;
        std::memcpy(write_ptr, uuidbuf, UUID_LEN); write_ptr += UUID_LEN;
        if (!obj_payload.empty()) {
            std::memcpy(write_ptr, obj_payload.data(), obj_payload.size());
        }

        // Clear dirty flag after snapshotting
        net.clear_dirty();
    }

    msg.payload = std::move(payload);
    msg.header.size = static_cast<uint32_t>(msg.payload.size());
    return msg;
}

void apply_full_snapshot(Scene& scene, const Message& msg)
{
    const auto& payload = msg.payload;
    if (payload.size() < sizeof(uint16_t))
        return;

    size_t offset = 0;
    uint16_t count = 0;
    std::memcpy(&count, payload.data() + offset, sizeof(count));
    offset += sizeof(count);

    for (uint16_t i = 0; i < count; ++i) {
        // need at least UUID + 6 floats + comp count
        if (offset + UUID_LEN + sizeof(float) * 6 + sizeof(uint16_t) > payload.size())
            break;

        char uuidbuf[UUID_LEN] = {};
        std::memcpy(uuidbuf, payload.data() + offset, UUID_LEN);
        offset += UUID_LEN;

        std::string uuid_str(uuidbuf);

        // Find existing object by NetworkIdentity uuid
        GameObject* found = nullptr;
        for (auto& go_ref : scene.game_objects()) {
            auto comp_opt = go_ref.get().get_component<NetworkIdentity>();
            if (!comp_opt) continue;
            if (comp_opt->get().uuid() == uuid_str) {
                found = &go_ref.get();
                break;
            }
        }

        // Let the GameObject parse its object-level fields and component payloads
        if (found) {
            found->deserialize(payload, offset);
        }
    }
}

void apply_delta_snapshot(Scene& scene, const Message& msg)
{
    // Delta format is identical per-entry to full snapshot but contains only changed objects
    const auto& payload = msg.payload;
    if (payload.size() < sizeof(uint16_t))
        return;

    size_t offset = 0;
    uint16_t count = 0;
    std::memcpy(&count, payload.data() + offset, sizeof(count));
    offset += sizeof(count);

    for (uint16_t i = 0; i < count; ++i) {
        // need at least UUID + comp count
        if (offset + UUID_LEN + sizeof(uint16_t) > payload.size())
            break;

        char uuidbuf[UUID_LEN] = {};
        std::memcpy(uuidbuf, payload.data() + offset, UUID_LEN);
        offset += UUID_LEN;

        std::string uuid_str(uuidbuf);

        // Find existing object by NetworkIdentity uuid
        GameObject* found = nullptr;
        for (auto& go_ref : scene.game_objects()) {
            auto comp_opt = go_ref.get().get_component<NetworkIdentity>();
            if (!comp_opt) continue;
            if (comp_opt->get().uuid() == uuid_str) {
                found = &go_ref.get();
                break;
            }
        }

        // Let the GameObject parse its object-level fields and component payloads
        if (found) {
            found->deserialize(payload, offset);
        }
    }
}

} // namespace snapshot
