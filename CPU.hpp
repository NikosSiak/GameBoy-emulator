//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_CPU_HPP
#define GAMEBOY_EMULATOR_CPU_HPP


#include <cstdint>

class CPU {

    // Registers (values from Power Up Sequence http://bgb.bircd.org/pandocs.htm#powerupsequence)
    uint8_t A = 0x01;
    uint16_t BC = 0x0013;
    uint16_t DE = 0x00D8;
    uint16_t HL = 0x014D;
    uint16_t pc = 0;
    uint16_t sp = 0xFFFE;

    // Flags
    bool Z = true; // Zero Flag becomes set (true) if the result of an operation has been zero
    bool N = false; // Add/Sub Flag: indicates whether the previous instruction has been an addition or subtraction
    bool H = true; // Half Carry Flag: carry for lower 4bits of the result
    bool C = true; // Carry Flag

public:
    int emulateInstruction(); // returns number of cycles needed for the instruction
};


#endif //GAMEBOY_EMULATOR_CPU_HPP
