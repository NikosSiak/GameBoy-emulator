//
// Created by nikos on 04-Oct-20.
//

#ifndef GAMEBOY_EMULATOR_TILE_HPP
#define GAMEBOY_EMULATOR_TILE_HPP

#include <vector>
#include <cstdint>

const uint8_t TILES_PER_LINE = 32;
const uint8_t TILE_HEIGHT_PX = 8;
const uint8_t TILE_WIDTH_PX = 8;

const uint16_t TILE_SET_ZERO_ADDRESS = 0x8000;
const uint16_t TILE_SET_ONE_ADDRESS = 0x8800;

const uint16_t TILE_MAP_ZERO_ADDRESS = 0x9800;
const uint16_t TILE_MAP_ONE_ADDRESS = 0x9C00;

class Memory;
enum class Colour;

class Tile {
    Colour buffer[8 * 2 * 8];

//    unsigned int pixelIndex(uint32_t x, uint32_t y);
    static std::vector<Colour> getPixelLine(uint8_t byte1, uint8_t byte2);

public:

    Tile(uint16_t tileAddress, Memory& memory, uint8_t sizeMultiplier);
    Colour getPixel(uint32_t x, uint32_t y);
};


#endif //GAMEBOY_EMULATOR_TILE_HPP
