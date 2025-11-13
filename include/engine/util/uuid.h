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

    }
}
