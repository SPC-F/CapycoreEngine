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
    std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>> try_get_spritesheet(const std::string& key) const;

    [[nodiscard]]
    std::expected<
        std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>,
        std::string> create_spritesheet_for(const std::string& source, const std::string& name, size_t from, size_t to);

    [[nodiscard]]
    std::optional<std::reference_wrapper<Texture>> try_get_texture(const std::string& sprite) const;

    [[nodiscard]]
    std::expected<std::reference_wrapper<Texture>, std::string> register_texture(
        const std::string& resource_name,
        const std::string& texture_name,
        size_t index);

    std::vector<std::reference_wrapper<Texture>> load_from_resource(
            const std::string& file,
            const std::string& name,
            int rows,
            int cols);

private:
    std::vector<std::unique_ptr<Texture>> textures_;
    std::map<std::string, std::vector<std::reference_wrapper<Texture>>> named_assets_;
};
