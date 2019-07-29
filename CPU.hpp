//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_CPU_HPP
#define GAMEBOY_EMULATOR_CPU_HPP


#include <cstdint>

class CPU {

    // Registers
    uint8_t A;
    uint16_t BC;
    uint16_t DE;
    uint16_t HL;
    uint16_t pc;
    uint16_t sp;

    // Flags
    bool Z; // Zero Flag becomes set (true) if the result of an operation has been zero
    bool N; // Add/Sub Flag: indicates whether the previous instruction has been an addition or subtraction
    bool H; // Half Carry Flag: carry for lower 4bits of the result
    bool C; // Carry Flag
};


#endif //GAMEBOY_EMULATOR_CPU_HPP
