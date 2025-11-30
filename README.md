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


## Linter

1. Install pre-commit

First, install pre-commit on your system. This tool is required to manage the hooks that will run before each commit.
```bash
pip install pre-commit
```

2. Install the Git hook

After installing pre-commit, run the following command to set up the hooks defined in the .pre-commit-config.yaml file for your project:
```bash
pre-commit install
```

This command will create the necessary Git hook configurations to ensure that clang-format is run automatically on staged files before each commit.

3. Manually run the hook (Optional)

If you want to manually run the hook and format all the files in your repository, you can do so with the following command:
```bash
pre-commit run --all-files
```

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