#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <engine/core/assetManager.h>
#include <engine/core/engine.h>

AssetManager::AssetManager()
    : textures_(), namedAssets_() {
}

std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>> AssetManager::try_get_resource(const std::string& key) const {
    if (const auto it = namedAssets_.find(key); it != namedAssets_.end()) {
        // const ref, not c ref. constructor functions for these things are called ref and cref respectively.
        return std::cref(it->second);
    }
    return std::nullopt;
}

std::expected<
        std::reference_wrapper<const std::vector<std::reference_wrapper<Texture>>>,
        std::string> AssetManager::create_texture_for(const std::string& source, const std::string& name, size_t from, size_t to){

}

std::vector<std::reference_wrapper<Texture>> AssetManager::load_from_resource(
        const std::string& file,
        const std::string& name,
        int rows,
        int cols) {
    const std::string filePath = "resources/" + file;

    const auto& rendererService = Engine::instance()
        .services
        ->getService<RenderingManager>()
        .get();

    SDL_Renderer* renderer = rendererService.getSDLRenderer();

    SDL_Surface* image = IMG_Load(filePath.c_str());
    SDL_Texture* textureSheet = SDL_CreateTextureFromSurface(renderer, image);
    SDL_DestroySurface(image);

    const auto width = float(textureSheet->w) / float(cols);
    const auto height = float(textureSheet->h) / float(rows);

    SDL_FRect src_rect{0, 0, width, height};
    const SDL_FRect dst_rect{0, 0, width, height};

    namedAssets_.emplace(name, std::vector<std::reference_wrapper<Texture>>());
    auto& resource = namedAssets_.at(name); // <-- reference to actual stored vector

    for (int r = 0; r < rows; ++r) {
        src_rect.x = 0;
        for (int c = 0; c < cols; ++c) {
            SDL_Texture* frame = SDL_CreateTexture(
                    renderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    static_cast<int>(width),
                    static_cast<int>(height));

            SDL_SetRenderTarget(renderer, frame);
            SDL_RenderTextureTiled(renderer, textureSheet, &src_rect, 1, &dst_rect);

            // Create a unique_ptr and store it in _textures
            auto newTexture = std::unique_ptr<Texture>(new Texture(frame));
            textures_.emplace_back(std::move(newTexture));

            // Add a reference to the stored Texture in the namedAssets vector
            resource.emplace_back(*textures_.back());

            src_rect.x += width;
        }
        src_rect.y += height;
    }

    SDL_DestroyTexture(textureSheet);
    SDL_SetRenderTarget(renderer, nullptr);

    return resource; // returns a copy of reference_wrapper vector
}