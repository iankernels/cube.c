cmake_minimum_required(VERSION 3.14)

project(cube C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wconversion -pedantic -O2)
endif()

find_package(SDL2 REQUIRED)

add_executable(cube cube.c)
target_link_libraries(cube PRIVATE SDL2::SDL2 m)

# For IDEs, set the working directory to the project dir
set_property(TARGET cube PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


