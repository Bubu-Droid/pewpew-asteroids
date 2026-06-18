cmake_minimum_required (VERSION 3.25)

project(asteroids)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++23")
set(source_dir "${PROJECT_SOURCE_DIR}/src")
set(deps_dir "${PROJECT_SOURCE_DIR}/deps")
# set(textures_dir "${PROJECT_SOURCE_DIR}/textures")

add_executable (asteroids
  "${source_dir}/main.cpp"
  "${source_dir}/asteroids.cpp"
  "${source_dir}/player.cpp"
  "${source_dir}/projectile.cpp"
)

if(WIN32)
target_include_directories(asteroids PRIVATE
  "${deps_dir}/raylib-windows/include"
)
target_link_directories(asteroids PRIVATE
  "${deps_dir}/raylib-windows/lib"
)
target_link_libraries(asteroids PRIVATE raylib opengl32 gdi32 winmm)
target_link_options(asteroids PRIVATE
  "-static" "-static-libgcc" "-static-libstdc++"
)

elseif(LINUX)
  if (NOT TARGET raylib)
    find_package(raylib 6.0 REQUIRED)
  endif()
  target_include_directories(asteroids PRIVATE
    "${deps_dir}/raylib/include"
  )
  target_link_directories(asteroids PRIVATE
    "${deps_dir}/raylib/lib"
  )
  target_link_libraries(asteroids PRIVATE raylib)
endif()


install(TARGETS asteroids DESTINATION bin)
# install(FILES
# "${textures_dir}/ship.png"
# "${textures_dir}/back.png"
#   DESTINATION textures
# )

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "asteroids")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
include(CPack)
