//
// Created by nikos on 29-Jul-19.
//

#include "CPU.hpp"

uint8_t CPU::readByteFromMemory(uint16_t address) {
    // TODO
}

void CPU::writeByteToMemory(uint16_t address, uint8_t value) {
    // TODO
}

void CPU::enableInterruptRegister() {
    // TODO
}

int CPU::emulateInstruction() { // returns number of cycles needed for the instruction

    uint8_t opcode = readByteFromMemory(pc++);

    switch (opcode) {
        case 0xC3: {    // JP nn: Jump to the absolute address specified by the operand nn. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
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
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            if ((opcode == 0xC2 && !Z) || (opcode == 0xCA && Z) || (opcode == 0xD2 && !C) || (opcode == 0xDA && C)) {
                pc = nn;
                return 16;
            }
            return 12;
        }
        case 0x18: {    // JP n: Jump to pc + n. n = signed int 8 bit
            int8_t n = static_cast<int8_t>(readByteFromMemory(pc++));
            pc += n;
            return 12;
        }
        case 0x20:      // JP NZ, n: Jump to pc + n if zero flag is false.  n is always read.  n = signed int 8 bit
        case 0x28:      // JP Z,  n: Jump to pc + n if zero flag is true.   n is always read.  n = signed int 8 bit
        case 0x30:      // JP NC, n: Jump to pc + n if carry flag is false. n is always read.  n = signed int 8 bit
        case 0x38: {    // JP C,  n: Jump to pc + n if carry flag is true.  n is always read.  n = signed int 8 bit
            int8_t n = static_cast<int8_t>(readByteFromMemory(pc++));
            if ((opcode == 0x20 && !Z) || (opcode == 0x28 && Z) || (opcode == 0x30 && !C) || (opcode == 0x38 && C)) {
                pc += n;
                return 12;
            }
            return 8;
        }
        case 0xCD: {    // CALL nn: Push pc to stack and jump to nn. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = nn;
            return 24;
        }
        case 0xC4:      // CALL NZ, nn: CALL nn if zero flag is false.  nn is always read. nn = unsigned int 16 bit. LSB first
        case 0xCC:      // CALL Z,  nn: CALL nn if zero flag is true.   nn is always read. nn = unsigned int 16 bit. LSB first
        case 0xD4:      // CALL NC, nn: CALL nn if carry flag is false. nn is always read. nn = unsigned int 16 bit. LSB first
        case 0xDC: {    // CALL C,  nn: CALL nn if carry flag is true.  nn is always read. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            if ((opcode == 0xC4 && !Z) || (opcode == 0xCC && Z) || (opcode == 0xD4 && !C) || (opcode == 0xDC && C)) {
                writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
                writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
                pc = nn;
                return 24;
            }
            return 12;
        }
        case 0xC0:      // RET NZ: Return from function if zero flag is false.  Return address is read only if condition is true
        case 0xC8:      // RET Z : Return from function if zero flag is true.   Return address is read only if condition is true
        case 0xD0:      // RET NC: Return from function if carry flag is false. Return address is read only if condition is true
        case 0xD8: {    // RET C : Return from function if carry flag is true.  Return address is read only if condition is true
            if ((opcode == 0xC0 && !Z) || (opcode == 0xC8 && Z) || (opcode == 0xD0 && !C) || (opcode == 0xD8 && C)) {
                uint8_t lsb = readByteFromMemory(sp++);
                uint8_t msb = readByteFromMemory(sp++);
                pc = (msb << 8) | lsb;
                return 20;
            }
            return 8;
        }
        case 0xC9:      // RET : Return from function
        case 0xD9: {    // RETI: Return from function and enable interrupts
            uint8_t lsb = readByteFromMemory(sp++);
            uint8_t msb = readByteFromMemory(sp++);
            pc = (msb << 8) | lsb;
            if (opcode == 0xD9) {
                enableInterruptRegister();
            }
            return 16;
        }
        case 0xC7: {    // RST 0x00: Puh pc tp stack and jump to address 0x0000
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0000;
            return 16;
        }
        case 0xCF: {    // RST 0x08: Puh pc tp stack and jump to address 0x0008
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0008;
            return 16;
        }
        case 0xD7: {    // RST 0x10: Puh pc tp stack and jump to address 0x0010
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0010;
            return 16;
        }
        case 0xDF: {    // RST 0x18: Puh pc tp stack and jump to address 0x0018
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0018;
            return 16;
        }
        case 0xE7: {    // RST 0x20: Puh pc tp stack and jump to address 0x0020
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0020;
            return 16;
        }
        case 0xEF: {    // RST 0x28: Puh pc tp stack and jump to address 0x0028
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0028;
            return 16;
        }
        case 0xF7: {    // RST 0x30: Puh pc tp stack and jump to address 0x0030
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0030;
            return 16;
        }
        case 0xFF: {    // RST 0x38: Puh pc tp stack and jump to address 0x0038
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0038;
            return 16;
        }

        default:
            return 0;
    }
}
