# Capycore Engine 101
The Capycore engine is a C++ game engine designed for 2D games. It is developed as a product of the Avans Hogeschool 
Systems Programming minor in C++ ('s-Hertogenbosch, Netherlands).

## How-to-run
1. Clone the capycore engine repo
2. Clone the freetype-win-build repo into the same root that you cloned the capycore engine into.
3. Build with CMake.

```shell
git clone <engine-repo-url>
git clone <freetype-win-build-repo-url>
cd CapycoreEngine

# Example using CMake with Ninja generator from CLion
cmake -G "Ninja" -S . -B build
```

Your structure should end up like this:
```
root
│
├── CapycoreEngine
│   └── ...
├── freetype-win-build
│   └── ...
└── Your-game-maybe?
    └── CMakeLists.txt
```

## Note for Linux users
Currently even Linux users have to do the copy thing for freetype. This is because the repository 
is primarily intended for Windows users who have a hard time building freetype from source. In 
this case it was done like this so Windows users do not have to build freetype themselves. It will 
be included in the engine so end-users have nothing to do with it.

In the future, we may make it so Linux users can just link the freetype library normally. This change 
may not affect Windows users in any way shape or form, meaning that Windows users will still be able to
use the prebuilt binaries from the freetype-win-build repository regardless.


## Clang-tidy
This project uses *clang-tidy* as its linter. Its configured through `.clang-tidy` and needs the command export from your build: `"CMAKE_EXPORT_COMPILE_COMMANDS": "ON"`.

We do utilize c++ 20 features so make sure that these are also used: `"CMAKE_CXX_STANDARD": "20","CMAKE_CXX_STANDARD_REQUIRED": "ON"`
For msys2 users: `pacman -S mingw-w64-ucrt-x86_64-clang-tools-extra`
For linux/mac: `something similar...`

A new target **[clang-tidy]** is defined in CMake that runs the linter on the entire `/src` directory. Due note that this simply lints and not fully builds. 
Simply add it to the existing build command and voila, it checks all.

`cmake --build build/default (build) --target clang_tidy`

If you install clang-tidy then you can also run it yourself for smaller checks:

`clang-tidy -p [BUILD-FOLDER-PATH] .\src\main.cpp`

---
An example of a preset (windows):
```json
{
    "version": 5,
    "configurePresets": [
        {
            "name": "gcc",
            "displayName": "GCC 15.2.0 x86_64-w64-mingw32 (ucrt64)",
            "description": "Using compilers: C = c:\\msys64\\ucrt64\\bin\\gcc.exe, CXX = c:\\msys64\\ucrt64\\bin\\g++.exe",
            "generator": "MinGW Makefiles",
            "binaryDir": "${sourceDir}/build/${presetName}",
            "cacheVariables": {
                "CMAKE_CXX_STANDARD": "20",
                "CMAKE_CXX_STANDARD_REQUIRED": "ON",
                "CMAKE_INSTALL_PREFIX": "${sourceDir}/install/${presetName}",
                "CMAKE_C_COMPILER": "c:/msys64/ucrt64/bin/gcc.exe",
                "CMAKE_CXX_COMPILER": "c:/msys64/ucrt64/bin/g++.exe",
                "CMAKE_BUILD_TYPE": "Debug",
                
                "ENABLE_TRACY": "ON",
                "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
            }
        }
    ],
    "buildPresets": [
        {
            "name": "gcc",
            "description": "",
            "displayName": "",
            "configurePreset": "gcc"
        },
        {
            "name": "tidy",
            "description": "",
            "displayName": "",
            "configurePreset": "gcc",
            "targets": [ "clang_tidy" ]
        }
    ]
}
```