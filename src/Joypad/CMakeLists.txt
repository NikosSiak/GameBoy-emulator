add_library(joypad STATIC Joypad.cpp Joypad.hpp)

target_link_libraries(joypad PUBLIC memory util)
