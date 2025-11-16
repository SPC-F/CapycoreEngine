#pragma once

#include <random>
#include <sstream>
#include <iomanip>
#include <array>
#include <iostream>

namespace uuid {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 255);

    std::string generate_uuid_v4() {
        std::array<uint8_t, 16> bytes;
        for (auto& b : bytes) {
            b = static_cast<uint8_t>(dis(gen));
        }

        bytes[6] = (bytes[6] & 0x0F) | 0x40;
        bytes[8] = (bytes[8] & 0x3F) | 0x80;

        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (size_t i = 0; i < bytes.size(); ++i) {
            ss << std::setw(2) << static_cast<int>(bytes[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9) ss << "-";
        }

        return ss.str();
    }
}
