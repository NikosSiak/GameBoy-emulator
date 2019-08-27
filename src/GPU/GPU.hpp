//
// Created by nikos on 12-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_GPU_HPP
#define GAMEBOY_EMULATOR_GPU_HPP

#include <cstdint>

class Memory;

enum class Colour {
    White,
    Light_Grey,
    Dark_Grey,
    Black
};

enum class VideoMode {
    ACCESS_OAM,
    ACCESS_VRAM,
    HBLANK,
    VBLANK,
};

const uint8_t CLOCKS_PER_HBLANK = 204; /* Mode 0 */
const uint8_t CLOCKS_PER_SCANLINE_OAM = 80; /* Mode 2 */
const uint8_t CLOCKS_PER_SCANLINE_VRAM = 172; /* Mode 3 */
const uint16_t CLOCKS_PER_SCANLINE =
        (CLOCKS_PER_SCANLINE_OAM + CLOCKS_PER_SCANLINE_VRAM + CLOCKS_PER_HBLANK);

const int CLOCKS_PER_VBLANK = 4560; /* Mode 1 */
const uint8_t SCANLINES_PER_FRAME = 144;
const int CLOCKS_PER_FRAME = (CLOCKS_PER_SCANLINE * SCANLINES_PER_FRAME) + CLOCKS_PER_VBLANK;

class GPU {

    Colour pixels[160][144];
    int scanLineCounter = 0;
    VideoMode current_mode = VideoMode::ACCESS_OAM;

    Memory &m_memory;

    bool isLCDEnabled();
    void drawScanLine();
    void drawBackground();
    void drawSprites();
    Colour getColour(uint8_t colourNum, uint16_t address);

public:
    GPU(Memory &memory);
    void updateGraphics(int cycles);
    Colour getPixelColour(int i, int j);
};


#endif //GAMEBOY_EMULATOR_GPU_HPP
