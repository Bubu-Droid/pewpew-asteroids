#!/usr/bin/env bash

# Mother of all installers :)

# xxd -i ./textures/ship.png >./src/ship_bytes.h
# xxd -i ./textures/back.png >./src/back_bytes.h

echo "Building for Linux..."

# Linux build
rm -rf ./build
mkdir build
cd ./build || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cpack
cp asteroids ../asteroids-linux
cd ..

echo "Building for Windows..."

# Windows build
rm -rf ./windows-build
mkdir windows-build
cd ./windows-build || exit
cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cpack -G NSIS
cp asteroids.exe ../asteroids-windows.exe
cd ..
