#include <engine/util/uuid.h>
#include <random>
#include <array>

namespace uuid {

    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_int_distribution<uint8_t> dis(0, 255);

    std::string generate_uuid_v4() {
        std::array<uint8_t, 16> bytes{};

        for (auto& b : bytes) {
            b = dis(gen);
        }

        bytes[6] = (bytes[6] & 0x0F) | 0x40;
        bytes[8] = (bytes[8] & 0x3F) | 0x80;

        char buffer[37];
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