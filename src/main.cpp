#include <engine/core/engine.h>
#include <engine/util/memory.h>

// Macro's for tracy based on the CMake option
#ifdef TRACY_ENABLE
#define TRACY_INIT() tracy_memory_init()
#define TRACY_SHUTDOWN() tracy_memory_shutdown()
#else
#define TRACY_INIT()
#define TRACY_SHUTDOWN()
#endif

int main() {
    TRACY_INIT();

    TRACY_SHUTDOWN();
    return 0;
}