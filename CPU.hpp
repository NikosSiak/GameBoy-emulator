//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_CPU_HPP
#define GAMEBOY_EMULATOR_CPU_HPP


#include <cstdint>

class CPU {

    // Registers (values from Power Up Sequence http://bgb.bircd.org/pandocs.htm#powerupsequence)
    // Registers can be used as 16bit registers by pairing them up in the following manner: AF, BC, DE, HL
    uint8_t A = 0x01;
    uint8_t B = 0x00;
    uint8_t C = 0x13;
    uint8_t D = 0x00;
    uint8_t E = 0xD8;
    uint8_t H = 0x01;
    uint8_t L = 0x4D;
    uint16_t pc = 0;
    uint16_t sp = 0xFFFE;

    // Flags. Register F
    bool FZ = true;  // Zero Flag becomes set (true) if the result of an operation has been zero
    bool FN = false; // Add/Sub Flag: indicates whether the previous instruction has been an addition or subtraction
    bool FH = true;  // Half Carry Flag: carry for lower 4bits of the result
    bool FC = true;  // Carry Flag

public:
    uint8_t getF();
    void setF(uint8_t value);
    uint16_t getBC();
    void setBC(uint16_t value);
    uint16_t getDE();
    void setDE(uint16_t value);
    uint16_t getHL();
    void setHL(uint16_t value);
    uint8_t readByteFromMemory(uint16_t address);
    void writeByteToMemory(uint16_t address, uint8_t value);
    void enableInterruptRegister();
    int emulateInstruction(); // returns number of cycles needed for the instruction
};


#endif //GAMEBOY_EMULATOR_CPU_HPP
