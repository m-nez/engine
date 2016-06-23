cmake_minimum_required(VERSION 3.5)
project(Engine C)

set(CMAKE_C_FLAGS "-g -Wall")

add_subdirectory(sdlgl)
add_executable(demo main.c)
target_link_libraries(demo LINK_PUBLIC sdlgl)
