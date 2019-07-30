//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_CPU_HPP
#define GAMEBOY_EMULATOR_CPU_HPP


#include <cstdint>

class CPU {

    // Registers
    uint8_t A = 0;
    uint16_t BC = 0;
    uint16_t DE = 0;
    uint16_t HL = 0;
    uint16_t pc = 0;
    uint16_t sp = 0;

    // Flags
    bool Z = false; // Zero Flag becomes set (true) if the result of an operation has been zero
    bool N = false; // Add/Sub Flag: indicates whether the previous instruction has been an addition or subtraction
    bool H = false; // Half Carry Flag: carry for lower 4bits of the result
    bool C = false; // Carry Flag

};


#endif //GAMEBOY_EMULATOR_CPU_HPP
