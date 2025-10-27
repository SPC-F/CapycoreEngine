cmake_minimum_required(VERSION 3.25)
project(CapycoreEngine)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
if(WIN32)
    set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../freetype-win-build")
    set(FREETYPE_INCLUDE_DIR "${FREETYPE_ROOT}/include/freetype2")
    set(FREETYPE_LIBRARY "${FREETYPE_ROOT}/lib/freetype.lib")
    set(FREETYPE_DLL "${FREETYPE_ROOT}/bin/freetype.dll")

    add_library(Freetype::Freetype UNKNOWN IMPORTED)
    set_target_properties(Freetype::Freetype PROPERTIES
        IMPORTED_LOCATION "${FREETYPE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
    )

    set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR}")
    set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARY}")
else()
    find_package(Freetype REQUIRED)
    set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}") # Provided by find_package
    set(FREETYPE_LIBRARIES Freetype::Freetype)
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
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# =========================================================
# Sources
# =========================================================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(engine ${ENGINE_SOURCES} ${IMGUI_SOURCES})

target_include_directories(engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(engine
    PRIVATE SDL3::SDL3 enet box2d
)

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
        ${ENet_SOURCE_DIR}/include
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
# Avoid SDL_main issues
# =========================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

# =========================================================
# Enable testing
# =========================================================
option(ENABLE_TESTS "Enable building tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests PRIVATE engine gtest_main)
    add_test(NAME all_tests COMMAND tests)
endif()