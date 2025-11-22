#pragma once

#include <vector>
#include <map>
#include <optional>

#include <engine/core/rendering/texture.h>
#include <engine/core/rendering/font.h>
#include <engine/core/iEngineService.h>
#include <engine/public/util/color.h>

/**
 * @brief Service responsible for managing game assets such as textures and spritesheets.
 *
 * The AssetService provides the functionality to load sprite resources from files, and register
 * named textures and spritesheets and spritesheets in the engine as well as retrieve them when needed.
 */
class AssetService : public IEngineService{
public:
    AssetService();
    AssetService(const AssetService&) = delete;
    AssetService& operator=(const AssetService&) = delete;

    AssetService(AssetService&&) = default; // allow move
    AssetService& operator=(AssetService&&) = default;

    /**
     * @brief Attempts to retrieve a spritesheet by its key.
     * @param key The key of the registered spritesheet resource.
     * @return An optional reference to the vector of textures if found, std::nullopt otherwise.
     */
    [[nodiscard]]
    std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>> try_get_spritesheet(const std::string& key) const;

    /**
     * @brief Creates and registers a spritesheet from a stored resource.
     * @param source The source file name containing the spritesheet.
     * @param name The name to register the spritesheet under.
     * @param from The starting index of the sprites in the spritesheet.
     * @param to The ending index of the sprites in the spritesheet.
     * @return A reference to the registered vector of textures representing the spritesheet.
     * @throws An exception if the resource cannot be found or indices are out of bounds.
     */
    std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>> create_spritesheet_for(const std::string& source, const std::string& name, size_t from, size_t to);

    /**
     * @brief Attempts to retrieve a texture by its sprite name.
     * @param sprite The name of the registered texture resource.
     * @return An optional reference to the texture if found, std::nullopt otherwise.
     */
    [[nodiscard]]
    std::optional<std::reference_wrapper<Texture>> try_get_texture(const std::string& sprite) const;

    /**
     * @brief Registers a texture with a given resource name and texture name.
     * @param resource_name The name of the resource file.
     * @param texture_name The name to register the texture under.
     * @param index The index of the texture in the resource file.
     * @return A reference to the registered texture.
     * @throws An exception if the resource cannot be found or index is out of bounds.
     */
    std::reference_wrapper<Texture> register_texture(
        const std::string& resource_name,
        const std::string& texture_name,
        size_t index);

    /**
     * @brief Loads textures from a resource file and splits them into a grid of sprites.
     * @param file The resource file name.
     * @param name The base name for the loaded textures.
     * @param rows The number of rows in the spritesheet.
     * @param cols The number of columns in the spritesheet.
     * @return A vector of references to the loaded textures.
     * @throws An exception if the resource cannot be found or loading fails.
     */
    std::vector<std::reference_wrapper<Texture>> load_from_resource(
        const std::string& file,
        const std::string& name,
        int rows,
        int cols);

    /**
     * @brief Retrieves the default white texture.
     * @return A reference to the default white texture.
     */
    std::reference_wrapper<Texture> get_default_texture();

    /**
     * @brief Attempts to retrieve a font by its name and size.
     * @param font_name The name of the font.
     * @param font_size The size of the font.
     * @return An optional reference to the font if found, std::nullopt otherwise.
     */
    std::optional<std::reference_wrapper<Font>> try_get_font(const std::string& font_name, int font_size) const;

    /**
     * @brief Registers a font with the given name and size.
     * @param font_name The name of the font.
     * @param font_size The size of the font.
     * @return A reference to the registered font.
     * @throws An exception if the font cannot be loaded.
     */
    std::reference_wrapper<Font> register_font(const std::string font_name, const std::string font_path, int font_size);

private:
    static constexpr const char * const resource_path = "resources/sprites/";

    std::vector<std::unique_ptr<Texture>> textures_;
    std::map<std::string, std::vector<std::reference_wrapper<Texture>>> named_assets_;
    std::map<std::string, std::unique_ptr<Font>> font_cache_;
};
