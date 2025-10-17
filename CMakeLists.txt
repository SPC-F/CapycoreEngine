cmake_minimum_required(VERSION 3.25)
project(CapycoreEngine)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# =========================================================
# SDL3
# =========================================================
set(SDL_VERSION 3.2.22)
set(SDL_TEST OFF CACHE BOOL "Disable SDL3 internal tests")
set(SDL_SHARED ON CACHE BOOL "Build SDL3 as shared library")

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-${SDL_VERSION}
)
FetchContent_MakeAvailable(SDL3)

# =========================================================
# SDL3_image
# =========================================================
set(SDL3IMAGE_VENDORED ON)
FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_image)

# =========================================================
# FreeType (local package install, sibling folder)
# =========================================================
set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../freetype-win-build")
set(FREETYPE_INCLUDE_DIR "${FREETYPE_ROOT}/include/freetype2")
set(FREETYPE_LIBRARY "${FREETYPE_ROOT}/lib/freetype.lib")
set(FREETYPE_DLL "${FREETYPE_ROOT}/bin/freetype.dll")

add_library(Freetype::Freetype UNKNOWN IMPORTED)
set_target_properties(Freetype::Freetype PROPERTIES
        IMPORTED_LOCATION "${FREETYPE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
)

# Pass these into SDL_ttf
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR}")
set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARY}")

# =========================================================
# SDL3_ttf (needs FreeType)
# =========================================================
set(SDL3TTF_VENDORED OFF)
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR}")
set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARY}")

FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG main
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

# ImGui sources (core + SDL3 + OpenGL3 backends)
set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
        src/main.cpp
        src/main.cpp
)

# =========================================================
# Sources
# =========================================================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp src/*.h
        lib/*.cpp lib/*.h
        include/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

# =========================================================
# Linking
# =========================================================

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${enet_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        #sdl + freetype
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        Freetype::Freetype
        #enet
        enet
        #box2d
        box2d
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

    # Copy freetype.dll too
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FREETYPE_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    # Copy resources
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()

# =========================================================
# Avoid SDL_main issues
# =========================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)