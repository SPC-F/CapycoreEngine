#include <engine/network/snapshot.h>
#include <engine/public/components/network_identity.h>

#include <cstdint>
#include <cstring>

NetworkIdentity::NetworkIdentity() : uuid_(), owner_uuid_(), dirty_(false) {}

NetworkIdentity::NetworkIdentity(const std::string& uuid)
    : uuid_(uuid), owner_uuid_(), dirty_(false) {}

const std::string& NetworkIdentity::uuid() const noexcept { return uuid_; }

void NetworkIdentity::set_uuid(const std::string& uuid) noexcept {
  uuid_ = uuid;
}

const std::string& NetworkIdentity::owner_uuid() const noexcept {
  return owner_uuid_;
}

void NetworkIdentity::set_owner_uuid(const std::string& owner) noexcept {
  owner_uuid_ = owner;
}

void NetworkIdentity::mark_dirty() noexcept { dirty_ = true; }

void NetworkIdentity::clear_dirty() noexcept { dirty_ = false; }

bool NetworkIdentity::is_dirty() const noexcept { return dirty_; }

void NetworkIdentity::on_serialize(std::vector<uint8_t>& out) const {
  snapshot::write_string(out, uuid_);
  snapshot::write_string(out, owner_uuid_);
}

void NetworkIdentity::on_deserialize(const std::vector<uint8_t>& data,
                                     size_t& offset) {
  if (!snapshot::read_string(data, offset, uuid_)) return;
  if (!snapshot::read_string(data, offset, owner_uuid_)) return;
}
