//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_CPU_HPP
#define GAMEBOY_EMULATOR_CPU_HPP


#include <cstdint>
#include <Memory/Memory.hpp>

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

    Memory &m_memory;

    // Flags. Register F: FZ FN FH FC 0 0 0 0
    bool FZ = true;  // Zero Flag becomes set (true) if the result of an operation has been zero
    bool FN = false; // Add/Sub Flag: indicates whether the previous instruction has been an addition or subtraction. true = sub. false = add
    bool FH = true;  // Half Carry Flag: carry for lower 4bits of the result
    bool FC = true;  // Carry Flag

    bool master_interrupt_switch = false;
    bool change_master_interrupt_switch = false;

    void addInstruction(uint8_t target);
    void subInstruction(uint8_t target);
    void andInstruction(uint8_t target);
    void orInstruction(uint8_t target);
    void xorInstruction(uint8_t target);
    void cpInstruction(uint8_t target);
    void incInstruction(uint8_t &target);
    void decInstruction(uint8_t &target);
    void addHLInstruction(uint16_t target);
    void swapInstruction(uint8_t &target);
    void rlcInstruction(uint8_t &target);
    void rlInstruction(uint8_t &target);
    void rrcInstruction(uint8_t &target);
    void rrInstruction(uint8_t &target);
    void slaInstruction(uint8_t &target);
    void sraInstruction(uint8_t &target);
    void srlInstruction(uint8_t &target);
    void bitInstruction(uint8_t target, int bit);
    void setInstruction(uint8_t &target, int bit);
    void resInstruction(uint8_t &target, int bit);

    void serveInterrupts(uint8_t interrupt);

public:
    CPU(Memory &memory);
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
    int emulateInstruction(); // returns number of cycles needed for the instruction
    void checkInterrupts();
};


#endif //GAMEBOY_EMULATOR_CPU_HPP
