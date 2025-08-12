cmake_minimum_required(VERSION 3.14)

project(cube C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wconversion -pedantic -O2)
endif()

add_executable(cube cube.c)

# Prefer SDL2 CMake package, fallback to pkg-config
find_package(SDL2 CONFIG QUIET)
if(SDL2_FOUND)
  target_link_libraries(cube PRIVATE SDL2::SDL2 m)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
  target_link_libraries(cube PRIVATE PkgConfig::SDL2 m)
endif()

# For IDEs, set the working directory to the project dir
set_property(TARGET cube PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


