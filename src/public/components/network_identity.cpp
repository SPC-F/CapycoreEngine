#include <engine/public/components/network_identity.h>

#include <cstring>
#include <cstdint>

NetworkIdentity::NetworkIdentity()
    : uuid_(), owner_uuid_(), dirty_(false)
{
}

NetworkIdentity::NetworkIdentity(const std::string& uuid)
    : uuid_(uuid), owner_uuid_(), dirty_(false)
{
}

const std::string& NetworkIdentity::uuid() const noexcept { return uuid_; }

void NetworkIdentity::set_uuid(const std::string& uuid) noexcept { uuid_ = uuid; }

const std::string& NetworkIdentity::owner_uuid() const noexcept { return owner_uuid_; }

void NetworkIdentity::set_owner_uuid(const std::string& owner) noexcept { owner_uuid_ = owner; }

void NetworkIdentity::mark_dirty() noexcept { dirty_ = true; }

void NetworkIdentity::clear_dirty() noexcept { dirty_ = false; }

bool NetworkIdentity::is_dirty() const noexcept { return dirty_; }

void NetworkIdentity::on_serialize_payload(std::vector<uint8_t>& out) const {
  // Serialize UUID and owner_uuid as: uuid_len(uint16_t) + uuid_bytes + 
  // owner_len(uint16_t) + owner_bytes
  uint16_t uuid_len = static_cast<uint16_t>(uuid_.size());
  uint16_t owner_len = static_cast<uint16_t>(owner_uuid_.size());

  size_t old_size = out.size();
  out.resize(old_size + sizeof(uuid_len) + uuid_len + sizeof(owner_len) +
             owner_len);

  uint8_t* write_ptr = out.data() + old_size;
  std::memcpy(write_ptr, &uuid_len, sizeof(uuid_len));
  write_ptr += sizeof(uuid_len);
  if (uuid_len > 0) {
    std::memcpy(write_ptr, uuid_.data(), uuid_len);
    write_ptr += uuid_len;
  }
  std::memcpy(write_ptr, &owner_len, sizeof(owner_len));
  write_ptr += sizeof(owner_len);
  if (owner_len > 0) {
    std::memcpy(write_ptr, owner_uuid_.data(), owner_len);
  }
}

void NetworkIdentity::on_deserialize_payload(const std::vector<uint8_t>& data,
                                             size_t& offset) {
  if (offset + sizeof(uint16_t) > data.size()) {
    return;
  }
  uint16_t uuid_len = 0;
  std::memcpy(&uuid_len, data.data() + offset, sizeof(uuid_len));
  offset += sizeof(uuid_len);

  if (offset + uuid_len > data.size()) {
    return;
  }
  uuid_.assign(reinterpret_cast<const char*>(data.data() + offset), uuid_len);
  offset += uuid_len;

  if (offset + sizeof(uint16_t) > data.size()) {
    return;
  }
  uint16_t owner_len = 0;
  std::memcpy(&owner_len, data.data() + offset, sizeof(owner_len));
  offset += sizeof(owner_len);

  if (offset + owner_len > data.size()) {
    return;
  }
  owner_uuid_.assign(reinterpret_cast<const char*>(data.data() + offset),
                     owner_len);
  offset += owner_len;
}
