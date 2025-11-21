#include <engine/util/memory.h>
#include <engine/core/engine.h>
#include <engine/network/multiplayer_service.h>

#include <iostream>

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

int main(int argc, char* argv[]) {
    tracy_init();

    try {
        auto& engine = Engine::instance();
        auto& multiplayer_service = engine.services->get_service<MultiplayerService>().get();

        multiplayer_service.set_connection_port(1024);

        if (strcmp(argv[1], "h") == 0) {
            multiplayer_service.set_host();
            multiplayer_service.set_max_clients(2);
            multiplayer_service.start_server();
        } else {
            multiplayer_service.set_client();
            multiplayer_service.connect("127.0.0.1");
        }

        while (multiplayer_service.get_connection_state() != ConnectionState::DISCONNECTED)
        {
            if (strcmp(argv[1], "h") == 0 && multiplayer_service.get_client_amount() >= 2)
                multiplayer_service.disconnect();
            else if (multiplayer_service.get_connection_state() == ConnectionState::DISCONNECTED)
                break;
            multiplayer_service.poll();
        }
    } catch (std::string err) {
        std::cout << err << std::endl;
    }

    Engine::quit();

    tracy_shutdown();
    return 0;
}