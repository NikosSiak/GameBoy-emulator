cmake_minimum_required(VERSION 3.14)
project(GameBoy_emulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(GameBoy_emulator main.cpp Memory.cpp Memory.hpp CPU.cpp CPU.hpp GameBoy.cpp GameBoy.hpp)
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})