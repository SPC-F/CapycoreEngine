#include <engine/util/memory.h>
#include <engine/core/engine.h>

// Macro's for tracy based on the CMake option
namespace {
    #ifdef TRACY_ENABLE
    inline void tracy_init() { tracy_memory_init(); }
    inline void tracy_shutdown() { tracy_memory_shutdown(); }
    #else
    inline void tracy_init() {}
    inline void tracy_shutdown() {}
    #endif
}

int main() {
    tracy_init();

    auto& engine = Engine::instance();
    Engine::quit();

    tracy_shutdown();
    return 0;
}