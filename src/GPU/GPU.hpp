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

class GPU {

    Colour pixels[160][144];
    int scanLineCounter = 456;

    Memory &m_memory;

    bool isLCDEnabled();
    void setLCDStatus();
    void drawScanLine();
    void drawBackground();
    void drawSprites();
    Colour getColour(uint8_t colourNum, uint16_t address);

public:
    GPU(Memory &memory);
    void updateGraphics(int cycles);
};


#endif //GAMEBOY_EMULATOR_GPU_HPP
