#include "engine/core/rendering/assetService.h"

#include <format>
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "engine/core/rendering/renderingService.h"
#include "engine/core/engine.h"

constexpr int white_texture_size = 32;
constexpr Uint32 white_pixel = 0xFFFFFFFF;

AssetService::AssetService() = default;

std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>> AssetService::try_get_spritesheet(const std::string& key) const {
    if (const auto it = named_assets_.find(key); it != named_assets_.end()) {
        // const ref, not c ref. constructor functions for these things are called ref and cref respectively.
        return std::cref(it->second);
    }
    return std::nullopt;
}

std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>> AssetService::create_spritesheet_for(
    const std::string& source,
    const std::string& name,
    size_t from,
    size_t to) {

    const auto maybe_resource = try_get_spritesheet(source);

    if(!maybe_resource.has_value()) {
        throw std::invalid_argument(std::format("The given argument for {} has no associated resource registered.", source));
    }

    const std::vector<std::reference_wrapper<Texture>>& resource = *maybe_resource;

    if (from >= resource.size() || to > resource.size()) {
        throw std::out_of_range("Invalid range for creating texture subset");
    }
    if (from > to) {
        throw std::out_of_range("'from' index must not be greater than 'to'");
    }

    named_assets_.emplace(
        name,
        std::vector(
                resource.begin() + static_cast<int>(from),
                resource.begin() + static_cast<int>(to)
        )
    );

    return named_assets_.at(name);
}

std::vector<std::reference_wrapper<Texture>> AssetService::load_from_resource(
    const std::string& file,
    const std::string& name,
    int rows,
    int cols) {

    const std::string file_path = AssetService::resource_path + file;

    const auto& renderer_service = Engine::instance()
        .services
        ->get_service<RenderingService>()
        .get();

    SDL_Renderer* renderer = renderer_service.renderer_
        ->sdl_renderer_
        .get();

    SDL_Surface* image = IMG_Load(file_path.c_str());
    if (!image) {
        SDL_Log("Failed to load image '%s': %s", file_path.c_str(), SDL_GetError());
        return {};
    }
    SDL_Texture* texture_sheet = SDL_CreateTextureFromSurface(renderer, image);
    SDL_DestroySurface(image);
    if (!texture_sheet) {
        SDL_Log("Failed to create texture from surface for '%s': %s", file_path.c_str(), SDL_GetError());
        return {};
    }

    const auto width = float(texture_sheet->w) / float(cols);
    const auto height = float(texture_sheet->h) / float(rows);

    SDL_FRect src_rect{0, 0, width, height};
    const SDL_FRect dst_rect{0, 0, width, height};

    named_assets_.emplace(name, std::vector<std::reference_wrapper<Texture>>());
    auto& resource = named_assets_.at(name); // <-- reference to actual stored vector

    for (int r = 0; r < rows; ++r) {
        src_rect.x = 0;
        for (int c = 0; c < cols; ++c) {
            SDL_Texture* frame = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                static_cast<int>(width),
                static_cast<int>(height));

            SDL_SetTextureScaleMode(frame, SDL_SCALEMODE_NEAREST);
            SDL_SetRenderTarget(renderer, frame);
            SDL_RenderTextureTiled(renderer, texture_sheet, &src_rect, 1, &dst_rect);

            // Create a unique_ptr and store it in _textures
            auto new_texture = std::unique_ptr<Texture>(new Texture(frame));
            textures_.emplace_back(std::move(new_texture));

            // Add a reference to the stored Texture in the namedAssets vector
            resource.emplace_back(*textures_.back());

            src_rect.x += width;
        }
        src_rect.y += height;
    }

    SDL_DestroyTexture(texture_sheet);
    SDL_SetRenderTarget(renderer, nullptr);

    return resource; // returns a copy of reference_wrapper vector
}

std::optional<std::reference_wrapper<Texture>> AssetService::try_get_texture(const std::string& sprite) const {
    auto const maybe_resource = this->try_get_spritesheet(sprite);
    if (!maybe_resource.has_value()) {
        return std::nullopt;
    }
    const auto resource = *maybe_resource;
    if (resource.get().empty()) {
        return std::nullopt;
    }
    return std::ref(resource.get().at(0));
}

std::reference_wrapper<Texture> AssetService::register_texture(
    const std::string& resource_name,
    const std::string& texture_name,
    const size_t index) {

    auto const maybe_resource = this->try_get_spritesheet(resource_name);
    if(!maybe_resource.has_value()) {
        throw std::invalid_argument(std::format("The given argument for {} has no associated resource registered.", resource_name));
    }

    const std::vector<std::reference_wrapper<Texture>>& resource = *maybe_resource;
    if (index >= resource.size()) {
        throw std::invalid_argument(std::format("Invalid range for creating texture subset"));
    }

    auto texture = resource.at(index);
    named_assets_.emplace(
        texture_name,
        std::vector{texture}
    );

    return resource.at(index);
}

std::reference_wrapper<Texture> AssetService::get_default_texture() {
    const auto maybe_texture = this->try_get_texture("default");
    
    if (maybe_texture.has_value()) {
        return maybe_texture->get();
    }

    const auto& renderer_service = Engine::instance()
        .services
        ->get_service<RenderingService>()
        .get();

    SDL_Renderer* renderer = renderer_service.renderer_
        ->sdl_renderer_
        .get();

    SDL_Texture* tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        white_texture_size,
        white_texture_size
    );

    if (!tex) {
        throw std::runtime_error("Failed to create white texture");
    }

    std::vector<Uint32> pixels(static_cast<size_t>(white_texture_size) * static_cast<size_t>(white_texture_size), white_pixel);

    SDL_UpdateTexture(tex, nullptr, pixels.data(), static_cast<int>(static_cast<size_t>(white_texture_size) * sizeof(Uint32)));
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    auto new_texture = std::unique_ptr<Texture>(new Texture(tex));
    textures_.emplace_back(std::move(new_texture));
    named_assets_.emplace("default", std::vector<std::reference_wrapper<Texture>>{*textures_.back()});

    return *textures_.back();
}

std::optional<std::reference_wrapper<Font>> AssetService::try_get_font(const std::string& font_name, int8_t font_size) const {
    const std::string key = font_name + "_" + std::to_string(font_size);
    auto it = font_cache_.find(key);

    if (it != font_cache_.end()) {
        return std::ref(*(it->second));
    }

    return std::nullopt;
}

std::reference_wrapper<Font> AssetService::register_font(const std::string font_name, const std::string font_path, int8_t font_size) {
    const std::string key = font_name + "_" + std::to_string(font_size);

    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>(
        TTF_OpenFont(font_path.c_str(), static_cast<int>(font_size)),
        &TTF_CloseFont
    );

    if (!font) {
        throw std::runtime_error("Failed to load font: " + font_name);
    }

    auto font_ptr = std::unique_ptr<Font>(new Font(font_name, font_size, font.release()));
    font_cache_.emplace(key, std::move(font_ptr));

    return *font_cache_.at(key);
}