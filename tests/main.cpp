// NOLINTFILE
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <SDL3/SDL.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_SetEnvironmentVariable(nullptr, "SDL_VIDEODRIVER", "dummy", true);
    SDL_SetEnvironmentVariable(nullptr, "SDL_AUDIODRIVER", "dummy", true);
    
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    }

    int result = Catch::Session().run(argc, argv);

    SDL_Quit();
    
    return result;
}
