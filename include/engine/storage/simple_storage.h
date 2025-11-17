#pragma once

#include <map>
#include <optional>
#include <string>
#include <variant>

class SimpleStorage {
public:
    using StorageKey = std::string;
    using StorageValue = std::variant<int, float, std::string>;

    static SimpleStorage& instance();

    /**
     * @brief Attempt to retrieve a stored value.
     *
     * Looks up the given key and returns its associated value if it
     * exists. Returns an empty optional when the key is not present,
     * allowing callers to detect missing entries.
     */
    std::optional<StorageValue> try_get(const StorageKey& key_name);

    /**
     * @brief Store or overwrite a value.
     *
     * Assigns the provided value to the specified key. Existing values
     * at the same key are replaced. This operation updates only the
     * in-memory session storage.
     */
    void set_value(const StorageKey& key_name, const StorageValue& value);

    /**
     * @brief Remove all stored entries.
     *
     * Clears the entire storage map, discarding all keys and values
     * currently held in memory. Does not trigger persistence by itself.
     */
    void delete_all();

    /**
     * @brief Remove a specific stored entry.
     *
     * Deletes the value associated with the given key without affecting
     * other entries. Safe to call even when the key does not exist.
     */
    void delete_key(const StorageKey& key_name);

    /**
     * @brief Persist current storage to disk or another backend.
     *
     * Saves all in-memory key–value pairs so they can be restored
     * later. Call this to maintain state across application runs.
     */
    void save();

protected:
    /**
     * @brief In-memory map of all loaded storage entries.
     *
     * Holds key–value pairs after load() is called, enabling fast lookup
     * and mutation without requiring disk access. Serves as the working
     * session state for all storage operations.
     */
    std::map<StorageKey, StorageValue> session_storage_;

private:
    /**
     * @brief Engine is the sole caller of load().
     *
     * Only the Engine class should invoke load() during game startup.
     * This ensures that session_storage_ is correctly populated from
     * persistent storage before any game logic accesses it.
     */
    friend class Engine;

    /**
     * @brief Load stored data into memory.
     *
     * Restores the session_storage_ from a persistent source if one
     * exists. Intended to be called during initialization by Engine
     * to rebuild the in-memory state for fast access.
     */
    void load();
};
