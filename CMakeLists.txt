cmake_minimum_required (VERSION 3.25)

project(asteroids)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++23")
set(source_dir "${PROJECT_SOURCE_DIR}/src")
set(deps_dir "${PROJECT_SOURCE_DIR}/deps")

add_executable (asteroids
  "${source_dir}/main.cpp"
  "${source_dir}/asteroids.cpp"
  "${source_dir}/player.cpp"
)

target_include_directories(asteroids PRIVATE
  "${deps_dir}/raylib/include"
)

target_link_directories(asteroids PRIVATE
  "${deps_dir}/raylib/lib"
)

target_link_libraries(asteroids PRIVATE raylib)
