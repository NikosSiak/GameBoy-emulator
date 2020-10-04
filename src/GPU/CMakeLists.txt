add_library(gpu STATIC GPU.cpp GPU.hpp Tile.cpp Tile.hpp Colour.hpp Colour.cpp)

target_link_libraries(gpu PUBLIC memory utils)
