// NOLINT
#include <engine/util/uuid.h>
#include <random>
#include <array>

constexpr size_t uuid_bytes = 16;
constexpr uint8_t version_mask_clear = 0x0F;
constexpr uint8_t version_v4_set    = 0x40;
constexpr uint8_t variant_mask_clear = 0x3F;
constexpr uint8_t variant_rfc4122_set = 0x80;
constexpr size_t uuid_string_len = 36;
constexpr size_t uuid_string_buffer = uuid_string_len + 1;

namespace uuid {

    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_int_distribution<uint8_t> dis(0, 255);

    std::string generate_uuid_v4() {
        std::array<uint8_t, uuid_bytes> bytes{};

        for (auto& b : bytes) {
            b = dis(gen);
        }

        bytes[6] = (bytes[6] & version_mask_clear) | version_v4_set;
        bytes[8] = (bytes[8] & variant_mask_clear) | variant_rfc4122_set;

        char buffer[uuid_string_buffer];
        snprintf(
            buffer, sizeof(buffer),
            "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            bytes[0], bytes[1], bytes[2], bytes[3],
            bytes[4], bytes[5],
            bytes[6], bytes[7],
            bytes[8], bytes[9],
            bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]
        );

        return buffer;
    }
}