//
// Created by nikos on 12-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_GPU_HPP
#define GAMEBOY_EMULATOR_GPU_HPP

#include <cstdint>

#define GAMEBOY_WIDTH  160
#define GAMEBOY_HEIGHT 144

class Memory;
enum class Colour;

class GPU {

    enum class Mode {
        ACCESS_OAM,
        ACCESS_VRAM,
        HBLANK,
        VBLANK
    };

    Colour pixels[GAMEBOY_WIDTH][GAMEBOY_HEIGHT]{};
    int scanLineCounter = 456;
    uint32_t cyclesCounter = 0;
    Mode currentMode = Mode::ACCESS_OAM;

    const uint32_t CLOCKS_PER_HBLANK = 204; /* Mode 0 */
    const uint32_t CLOCKS_PER_SCANLINE_OAM = 80; /* Mode 2 */
    const uint32_t CLOCKS_PER_SCANLINE_VRAM = 172; /* Mode 3 */
    const uint32_t CLOCKS_PER_SCANLINE =
            (CLOCKS_PER_SCANLINE_OAM + CLOCKS_PER_SCANLINE_VRAM + CLOCKS_PER_HBLANK);

    const uint32_t CLOCKS_PER_VBLANK = 4560; /* Mode 1 */
    const uint32_t SCANLINES_PER_FRAME = 144;
    const uint32_t CLOCKS_PER_FRAME = (CLOCKS_PER_SCANLINE * SCANLINES_PER_FRAME) + CLOCKS_PER_VBLANK;

    Memory &m_memory;

    bool isLCDEnabled();
    void setLCDStatus();
    void drawScanLine();
    void drawBackground();
    void drawWindow();
    void drawSprites();
//    Colour getColour(uint8_t colourNum, uint16_t address);

public:
    GPU(Memory &memory);
    void updateGraphics(int cycles);
    Colour getPixelColour(int i, int j);
};


#endif //GAMEBOY_EMULATOR_GPU_HPP
