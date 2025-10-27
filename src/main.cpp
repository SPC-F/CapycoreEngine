#include <engine/core/engine.h>
#include <engine/util/memory.h>

int main() {
    tracy_memory_init();

    Engine engine;

    tracy_memory_shutdown();
    return 0;
}