cmake_minimum_required(VERSION 3.14)
project(GameBoy_emulator)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(src)

add_subdirectory(src)

add_executable(GameBoy_emulator src/main/main.cpp)
target_link_libraries(GameBoy_emulator PRIVATE main ${SDL2_LIBRARIES})
