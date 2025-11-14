#pragma once

#include <unordered_map>
#include <string>
#include <any>

/**
 * @brief A class containing the current state of all networking objects.
 *
 * The class manages the current network state of all networking objects and syncs them
 * with the other networking peers.
 */
class NetworkState {
public:
    void set(const std::string& key, const std::any& value) noexcept;

    template<typename T>
    T get(const std::string& key);

    const bool contains(const std::string& key) const;

    /* @brief Serialize the current network state and return it. */
    std::string serialize();

    /* @brief Deserialize the new network state and save it. */
    void deserializer(std::string& data);

private:
    std::unordered_map<std::string, std::any> data;

    void append_to_buffer();
};
