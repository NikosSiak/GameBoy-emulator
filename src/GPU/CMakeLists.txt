add_library(gpu STATIC GPU.cpp GPU.hpp)

target_link_libraries(gpu PUBLIC memory utils)
