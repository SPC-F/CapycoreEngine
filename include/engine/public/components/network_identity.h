#pragma once

#include <engine/public/component.h>

#include <string>

/**
 * @brief Component that gives a GameObject a network identity (UUID) and dirty flag.
 *
 * Attach this to any GameObject that should be replicated. Systems or user code
 * should call `mark_dirty()` when the object's networked state changes.
 */
class NetworkIdentity : public Component {
private:
    std::string uuid_;
    std::string owner_uuid_;
    bool dirty_{false};

public:
    NetworkIdentity();
    explicit NetworkIdentity(const std::string& uuid);

    [[nodiscard]] const std::string& uuid() const noexcept;
    void set_uuid(const std::string& uuid) noexcept;

    [[nodiscard]] const std::string& owner_uuid() const noexcept;
    void set_owner_uuid(const std::string& owner) noexcept;

    void mark_dirty() noexcept;
    void clear_dirty() noexcept;
    [[nodiscard]] bool is_dirty() const noexcept;

    // Component interface
    void update(float /*dt*/) override {}
    void on_attach() override {}
    void on_detach() override {}
    void on_serialize() override {}
    void on_deserialize() override {}
    void on_serialize_payload(std::vector<uint8_t>& out) const override;
    void on_deserialize_payload(const std::vector<uint8_t>& data,
                                size_t& offset) override;
    std::string type_name() const override { return "NetworkIdentity"; }
};
