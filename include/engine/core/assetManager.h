#pragma once

#include <vector>
#include <map>
#include <expected>
#include <optional>

#include <engine/core/rendering/texture.h>
#include <engine/core/iEngineService.h>

class AssetManager : public IEngineService{
public:
    AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    AssetManager(AssetManager&&) = default; // allow move
    AssetManager& operator=(AssetManager&&) = default;

    [[nodiscard]]
    std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>> try_get_resource(const std::string& key) const;

    [[nodiscard]]
    std::expected<
            std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>,
            std::string> create_texture_for(const std::string& source, const std::string& name, size_t from, size_t to);

    std::vector<std::reference_wrapper<Texture>> load_from_resource(
            const std::string& file,
            const std::string& name,
            int rows,
            int cols);
private:
    std::vector<std::unique_ptr<Texture>> textures_;
    std::map<std::string, std::vector<std::reference_wrapper<Texture>>> namedAssets_;
};
