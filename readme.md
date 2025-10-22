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

### Note for Linux users
Currently even Linux users have to do the copy thing for freetype. This is because the repository 
is primarily intended for Windows users who have a hard time building freetype from source. In 
this case it was done like this so Windows users do not have to build freetype themselves. It will 
be included in the engine so end-users have nothing to do with it.

In the future, we may make it so Linux users can just link the freetype library normally. This change 
may not affect Windows users in any way shape or form, meaning that Windows users will still be able to
use the prebuilt binaries from the freetype-win-build repository regardless.