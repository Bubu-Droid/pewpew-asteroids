#!/usr/bin/env bash

# Mother of all installers :)

echo "Building for Linux..."

# Linux build
rm -rf ./build
mkdir build
cd ./build || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cpack -G TGZ
cd ..

echo "Building for Windows..."

# Windows build
rm -rf ./windows-build
mkdir windows-build
cd ./windows-build || exit
cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cpack -G NSIS
cd ..
