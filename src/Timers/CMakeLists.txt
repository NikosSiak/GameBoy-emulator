add_library(timers STATIC Timers.cpp Timers.hpp)

target_link_libraries(timers PUBLIC memory utils)
