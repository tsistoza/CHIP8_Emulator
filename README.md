# INSTALLATION

## PACKAGES REQUIRED

1. CMake Version 3.23+
2. SDL3 Library
3. g++ compiler (C++20+) - USE MSYS2/UCRT64 OVER MSVC IF ON WINDOWS

## COMMAND LINE

1. Clone the Repository, ensure you are in the directory, and in a terminal enter these commands sequentially
2. ``cmake --presets buildProject``
3. ``cmake --build build``
4. ``cmake --install build``
5. In the CURR_DIR/install/bin is the executable
6. The build will stop if you do not meet or have the packages required