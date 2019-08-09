add_library(bus STATIC Bus.cpp Bus.hpp)

target_link_libraries(bus PUBLIC memory)
