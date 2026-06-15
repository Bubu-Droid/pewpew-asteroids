#!/usr/bin/env bash

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -B build && cmake --build build
