#pragma once

#include <map>
#include <optional>
#include <string>
#include <variant>
#include <stdexcept>

class SimpleStorage {
public:
    using StorageKey = std::string;
    using StorageValue = std::variant<int, float, std::string>;

    /**
      So usually I would not include comments about this. But this is some more complex stuff here be dragons,
      so I will be very explicit about what is going on here.

      Our storage is working with something called an invariant. An invariant is something we can see as a
      tagged union of sorts but a bit different. An invariant is essentially a type that says "I can be any of the following types".
      We know this from discriminated unions used in F# or free unions from TypeScript.

      The overloaded struct is a trick that basically boils down to the following:
          "Be a structure that overloads the call type for anything that is inside of you".

      In our case, its type may as well just be:
      struct overloaded {
          int operator()(int);
          bool operator()(bool);
          std::string operator()(std::string);
      }
      We just use templates to generate this type for us based on the shape of the invariant passed, that is all.
      In C++ 26 we have this type included in the base library. We have to write it manually here.
  */
    template<class... TSelector>
    struct overloaded : TSelector... {
        using TSelector::operator()...;
    };

    /**
     *
     * @tparam T expected_type
     * @param key storage key=
     * @param default_value the default value
     * @return the value contained inside the variant
     */
    template<typename T>
    T get_value_or_default(const std::string &key, T default_value) {
        // So we provide the overloads for the () operator here. Every version of it takes in x and returns T or throws...
        return std::visit(overloaded{
                              [](const T value) -> T {
                                  return value;
                              },
                              [](auto) -> T {
                                  throw std::invalid_argument{"invalid value for T in settings::value_of"};
                              },
                          }, instance().try_get(key).value_or(default_value));
    }

    static SimpleStorage &instance();

    /**
     * @brief Attempt to retrieve a stored value.
     *
     * Looks up the given key and returns its associated value if it
     * exists. Returns an empty optional when the key is not present,
     * allowing callers to detect missing entries.
     */
    std::optional<StorageValue> try_get(const StorageKey &key_name);

    /**
     * @brief Store or overwrite a value.
     *
     * Assigns the provided value to the specified key. Existing values
     * at the same key are replaced. This operation updates only the
     * in-memory session storage.
     */
    void set_value(const StorageKey &key_name, const StorageValue &value);

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
    void delete_key(const StorageKey &key_name);

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

    SimpleStorage() = default;

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
