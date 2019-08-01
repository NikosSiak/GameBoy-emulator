//
// Created by nikos on 29-Jul-19.
//

#include "CPU.hpp"

uint8_t CPU::readFromMemory() {
    // TODO
}

int8_t CPU::signed8(uint8_t num) {
    // TODO
}

int CPU::emulateInstruction() { // returns number of cycles needed for the instruction

    uint8_t opcode = readFromMemory();

    switch (opcode) {
        case 0xC3: {    // JP u16: 0xC3 + LSB of nn + MSB of nn. Unconditional jump to the absolute address specified by the operand nn
            pc++;
            uint8_t lsb = readFromMemory();
            pc++;
            uint8_t msb = readFromMemory();
            pc = (msb << 8) | lsb;
            return 16;
        }
        case 0xE9: {    // JP HL: 0xE9 + LSB of nn + MSB of nn. Unconditional jump to the absolute address specified by the register HL
            pc = HL;
            return 4;
        }
        case 0xC2:      // JP NZ, nn: Jump to nn if zero flag is false.  nn is always read.  nn = unsigned int 16 bit
        case 0xCA:      // JP Z,  nn: Jump to nn if zero flag is true.   nn is always read.  nn = unsigned int 16 bit
        case 0xD2:      // JP NC, nn: Jump to nn if carry flag is false. nn is always read.  nn = unsigned int 16 bit
        case 0xDA: {    // JP C,  nn: Jump to nn if carry flag is true.  nn is always read.  nn = unsigned int 16 bit
            pc++;
            uint8_t lsb = readFromMemory();
            pc++;
            uint8_t msb = readFromMemory();
            uint16_t nn = (msb << 8) | lsb;
            if ((opcode == 0xC2 && !Z) || (opcode == 0xCA && Z) || (opcode == 0xD2 && !C) || (opcode == 0xDA && C)){
                pc = nn;
                return 16;
            }
            return 12;
        }
        case 0x18: {    // JP n: Jump to pc + n. n = signed int 8 bit
            pc++;
            pc += signed8(readFromMemory());
            return 12;
        }
        case 0x20:      // JP NZ, n: Jump to pc + n if zero flag is false.  n is always read.  n = signed int 8 bit
        case 0x28:      // JP Z,  n: Jump to pc + n if zero flag is true.   n is always read.  n = signed int 8 bit
        case 0x30:      // JP NC, n: Jump to pc + n if carry flag is false. n is always read.  n = signed int 8 bit
        case 0x38: {    // JP C,  n: Jump to pc + n if carry flag is true.  n is always read.  n = signed int 8 bit
            pc++;
            int8_t n = signed8(readFromMemory());
            if ((opcode == 0x20 && !Z) || (opcode == 0x28 && Z) || (opcode == 0x30 && !C) || (opcode == 0x38 && C)){
                pc = n;
                return 12;
            }
            return 8;
        }
    }


}
