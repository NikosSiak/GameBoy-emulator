//
// Created by nikos on 04-Oct-20.
//

#include <GPU/Tile.hpp>
#include <GPU/Colour.hpp>
#include <Memory/Memory.hpp>
#include <utils/bitoperations.hpp>

#define PIXEL_INDEX(x, y) ((y) * 8 + (x))

Tile::Tile(uint16_t tileAddress, Memory &memory, uint8_t sizeMultiplier) {
    for (uint8_t i = 0; i < TILE_WIDTH_PX; i++) {
        for (uint8_t j = 0; j < TILE_HEIGHT_PX * sizeMultiplier; j++) {
            buffer[PIXEL_INDEX(i, j)] = Colour::White;
        }
    }

    for (uint8_t tileLine = 0; tileLine < TILE_HEIGHT_PX * sizeMultiplier; tileLine++) {
        uint32_t indexIntoTile = 2 * tileLine;
        uint16_t lineStart = tileAddress + indexIntoTile;

        uint8_t pixels1 = memory.readByte(lineStart);
        uint8_t pixels2 = memory.readByte(lineStart + 1);

        std::vector<Colour> pixelLine = getPixelLine(pixels1, pixels2);

        for (uint32_t x = 0; x < TILE_WIDTH_PX; x++) {
            buffer[PIXEL_INDEX(x, tileLine)] = pixelLine[x];
        }
    }
}

Colour Tile::getPixel(uint32_t x, uint32_t y) {
    return buffer[PIXEL_INDEX(x, y)];
}

std::vector<Colour> Tile::getPixelLine(uint8_t byte1, uint8_t byte2) {
    std::vector<Colour> pixelLine;

    for (uint8_t i = 0; i < 8; i++) {
        Colour colour = getColour(static_cast<uint8_t>((getBit(byte2, 7-i) << 1) | getBit(byte1, 7-i)));
        pixelLine.push_back(colour);
    }

    return pixelLine;
}
