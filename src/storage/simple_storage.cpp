#include <filesystem>
#include <fstream>
#include <iostream>

#include <engine/storage/simple_storage.h>

namespace {
    std::filesystem::path save_path()
    {
        return std::filesystem::path("saved") / "simple.dat";
    }

    std::string escape_string(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());
        for (const char c : s) {
            if (c == '\\' || c == '"') {
                out.push_back('\\');
            }
            out.push_back(c);
        }
        return out;
    }

    std::string unescape_string(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '\\' && i + 1 < s.size()) {
                ++i; // Skip escape
            }
            out.push_back(s[i]);
        }
        return out;
    }
}

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
    const auto path = save_path();
    std::filesystem::create_directories(path.parent_path());

    std::ofstream out(path, std::ios::trunc);
    if (!out) {
        return;
    }

    out << "# WARNING: This file is managed by the engine.\n"
        << "# Manual edits are strongly discouraged. Incorrect changes can\n"
        << "# irreversibly damage your save data. Modify at your own risk—no\n"
        << "# support will be provided for tampered files.\n";

    for (const auto& [key, value] : session_storage_) {
        out << key << "=";
        // Not the most beautiful, but it gets the job done. Furthermore, it's
        // not on the roadmap to extend past these types.
        if (std::holds_alternative<int>(value)) {
            out << std::get<int>(value);
        } else if (std::holds_alternative<float>(value)) {
            out << std::get<float>(value);
        } else if (std::holds_alternative<std::string>(value)) {
            out << "\"" << escape_string(std::get<std::string>(value)) << "\"";
        }
        out << "\n";
    }
}

void SimpleStorage::load()
{
    session_storage_.clear();

    const auto path = save_path();
    std::ifstream in(path);
    if (!in) {
        return; // No file, nothing to load
    }

    std::string line;
    size_t line_number = 0;

    while (std::getline(in, line)) {
        ++line_number;

        if (line.empty()) {
            continue;
        }

        // Ignore comment lines exactly starting with '#'
        if (!line.empty() && line[0] == '#') {
            continue;
        }

        // Find '=' exactly; no trimming, no recovery
        const auto eq_pos = line.find('=');
        if (eq_pos == std::string::npos) {
            std::cerr << "[SimpleStorage] Error: malformed line " << line_number
                      << " (no '=' found): " << line << "\n";
            continue;
        }

        // Raw key and value as-is
        std::string key = line.substr(0, eq_pos);
        std::string val = line.substr(eq_pos + 1);

        if (key.empty()) {
            std::cerr << "[SimpleStorage] Error: empty key on line " << line_number << "\n";
            continue;
        }
        if (val.empty()) {
            std::cerr << "[SimpleStorage] Error: empty value on line " << line_number << "\n";
            continue;
        }

        // Parse string literal
        if (val.size() >= 2 && val.front() == '"' && val.back() == '"') {
            const auto inner = val.substr(1, val.size() - 2);
            session_storage_[key] = unescape_string(inner);
            continue;
        }

        // Parse float if it contains a dot
        if (val.find('.') != std::string::npos) {
            try {
                session_storage_[key] = std::stof(val);
            } catch (...) {
                std::cerr << "[SimpleStorage] Error: invalid float on line "
                          << line_number << ": " << val << "\n";
            }
            continue;
        }

        // Otherwise parse int
        try {
            session_storage_[key] = std::stoi(val);
        } catch (...) {
            std::cerr << "[SimpleStorage] Error: invalid int on line "
                      << line_number << ": " << val << "\n";
        }
    }
}
