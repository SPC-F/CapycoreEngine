#include <engine/storage/simple_storage.h>

SimpleStorage& SimpleStorage::instance()
{
    static SimpleStorage inst;
    return inst;
}

std::optional<SimpleStorage::StorageValue> SimpleStorage::try_get(const StorageKey& key_name)
{
    if (const auto it = session_storage_.find(key_name); it != session_storage_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void SimpleStorage::set_value(const StorageKey& key_name, const StorageValue& value)
{
    session_storage_[key_name] = value;
}

void SimpleStorage::delete_all()
{
    session_storage_.clear();
}

void SimpleStorage::delete_key(const StorageKey& key_name)
{
    session_storage_.erase(key_name);
}

void SimpleStorage::save()
{
    // TODO: implement persistence logic
}

void SimpleStorage::load()
{
    // TODO: implement loading logic
}
