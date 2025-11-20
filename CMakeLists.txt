cmake_minimum_required(VERSION 3.25)
project(CapycoreEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# =========================================================
# SDL3
# =========================================================
set(SDL_TEST OFF CACHE BOOL "Disable SDL3 internal tests")
set(SDL_SHARED ON CACHE BOOL "Build SDL3 as shared library")

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.24
)
FetchContent_MakeAvailable(SDL3)

# =========================================================
# SDL3_image
# =========================================================
set(SDL3IMAGE_VENDORED ON)
FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.2.4
)
FetchContent_MakeAvailable(SDL3_image)


# =========================================================
# FreeType (local package install, sibling folder)
# =========================================================
if(NOT TARGET Freetype::Freetype)
        if(NOT DEFINED FREETYPE_ROOT)
                set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../freetype-win-build" CACHE PATH "" FORCE)
        endif()
        if(NOT DEFINED FREETYPE_INCLUDE_DIR)
                set(FREETYPE_INCLUDE_DIR "${FREETYPE_ROOT}/include/freetype2" CACHE PATH "" FORCE)
        endif()
        if(NOT DEFINED FREETYPE_LIBRARY)
                set(FREETYPE_LIBRARY "${FREETYPE_ROOT}/lib/freetype.lib" CACHE FILEPATH "" FORCE)
        endif()

        add_library(Freetype::Freetype UNKNOWN IMPORTED)
        set_target_properties(Freetype::Freetype PROPERTIES
                IMPORTED_LOCATION "${FREETYPE_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
        )

        # Also update legacy variables for find_package compatibility
        set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR}" CACHE PATH "" FORCE)
        set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARY}" CACHE FILEPATH "" FORCE)
endif()
# =========================================================
# SDL3_ttf (needs FreeType)
# =========================================================
set(SDL3TTF_VENDORED OFF)
FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-3.2.2
)

FetchContent_MakeAvailable(SDL3_ttf)

# =========================================================
# ENet
# =========================================================
set(ENET_VERSION 1.3.18)
FetchContent_Declare(
        ENet
        URL http://enet.bespin.org/download/enet-${ENET_VERSION}.tar.gz
)
FetchContent_MakeAvailable(ENet)

# =========================================================
# Box2D (from GitHub, built automatically)
# =========================================================
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.1
)
FetchContent_MakeAvailable(box2d)

# =========================================================
# ImGui (Dear ImGui) via FetchContent
# =========================================================
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.2b # Use a stable release tag
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

# =========================================================
# Testing
# =========================================================
Set("ENABLE_TESTS" ON CACHE BOOL "Enable building tests")
if (ENABLE_TESTS)
        FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1 # or a later release
        )
        FetchContent_MakeAvailable(Catch2)
endif()

# =========================================================
# Sources
# =========================================================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(engine ${ENGINE_SOURCES} ${IMGUI_SOURCES})

target_include_directories(engine
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE 
        ${enet_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR} 
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(engine
    PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        Freetype::Freetype
        enet 
        box2d
)

# Link DbgHelp on Windows
if(WIN32)
        target_link_libraries(engine PRIVATE DbgHelp)
endif()

# =========================================================
# Tracy Profiler
# =========================================================
option(ENABLE_TRACY "Enable Tracy profiler integration" ON)

if(ENABLE_TRACY)
    FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.12.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(tracy)

    target_include_directories(engine PUBLIC ${tracy_SOURCE_DIR}/public)
    target_sources(engine PRIVATE ${tracy_SOURCE_DIR}/public/TracyClient.cpp)
    target_compile_definitions(engine PUBLIC TRACY_ENABLE TRACY_MEMORY_CALLSTACK=16)
endif()

# =========================================================
# Linking
# =========================================================
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        engine
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        Freetype::Freetype
        enet
        box2d
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${enet_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# =========================================================
# Post-build: copy required DLLs to executable folder (Windows)
# =========================================================
if(WIN32)
    foreach(lib SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${lib}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()

    # copy box2d dll if built (located at box2d-build/bin)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${box2d_BINARY_DIR}/bin/libbox2dd.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    # copy catch2 dll (located at Catch2-build/src)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${Catch2_BINARY_DIR}/src/libCatch2d.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FREETYPE_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()

# =========================================================
# Clang-Tidy target
# =========================================================
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
    )
    
    list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/tests/.*")
    list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/util/memory.*")

    add_custom_target(
        clang_tidy
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${PROJECT_SOURCES} --warnings-as-errors=*
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on the entire project..."
    )
endif()

# =========================================================
# Avoid SDL_main issues
# =========================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

# Tracy
if(ENABLE_TRACY)
        target_compile_definitions(engine PUBLIC TRACY_ENABLE TRACY_MEMORY_CALLSTACK=16)
endif()

# =========================================================
# Enable testing
# =========================================================
if(ENABLE_TESTS)
        file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
        list(FILTER TEST_SOURCES EXCLUDE REGEX ".*/main.cpp$")

        add_executable(tests
                tests/main.cpp
                ${TEST_SOURCES}
        )
        target_compile_definitions(tests PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

        target_link_libraries(tests
        PRIVATE
                engine
                Catch2::Catch2
        )

        target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")

        enable_testing()
        add_test(NAME engine_tests COMMAND tests)
endif()