add_library(emulator STATIC GameBoy.cpp GameBoy.hpp)

target_link_libraries(emulator PUBLIC cpu memory timers)
