add_library(memory STATIC Memory.cpp Memory.hpp)

target_link_libraries(memory PUBLIC utils)
