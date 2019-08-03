//
// Created by nikos on 29-Jul-19.
//

#include "CPU.hpp"

uint16_t CPU::getBC() {
    return (B << 8) | C;
}

void CPU::setBC(uint16_t value) {
    B = value >> 8;
    C = value & 0x00FF;
}

uint16_t CPU::getDE() {
    return (D << 8) | E;
}

void CPU::setDE(uint16_t value) {
    D = value >> 8;
    E = value & 0x00FF;
}

uint16_t CPU::getHL() {
    return (H << 8) | L;
}

void CPU::setHL(uint16_t value) {
    H = value >> 8;
    L = value & 0x00FF;
}

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
        case 0x06: {    // LD B, n: Load n to register B. n = unsigned int 8 bit.
            B = readByteFromMemory(pc++);
            return 8;
        }
        case 0x0E: {    // LD C, n: Load n to register B. n = unsigned int 8 bit.
            C = readByteFromMemory(pc++);
            return 8;
        }
        case 0x16: {    // LD D, n: Load n to register B. n = unsigned int 8 bit.
            D = readByteFromMemory(pc++);
            return 8;
        }
        case 0x1E: {    // LD E, n: Load n to register B. n = unsigned int 8 bit.
            E = readByteFromMemory(pc++);
            return 8;
        }
        case 0x26: {    // LD H, n: Load n to register B. n = unsigned int 8 bit.
            H = readByteFromMemory(pc++);
            return 8;
        }
        case 0x2E: {    // LD L, n: Load n to register B. n = unsigned int 8 bit.
            L = readByteFromMemory(pc++);
            return 8;
        }
        case 0x7F: {    // LD A, A: Load A to A
            return 4;
        }
        case 0x78: {    // LD A, B: Load B to A
            A = B;
            return 4;
        }
        case 0x79: {    // LD A, C: Load C to A
            A = C;
            return 4;
        }
        case 0x7A: {    // LD A, D: Load D to A
            A = D;
            return 4;
        }
        case 0x7B: {    // LD A, E: Load E to A
            A = E;
            return 4;
        }
        case 0x7C: {    // LD A, H: Load H to A
            A = H;
            return 4;
        }
        case 0x7D: {    // LD A, L: Load L to A
            A = L;
            return 4;
        }
        case 0x7E: {    // LD A, (HL): Load memory[HL] to A
            A = readByteFromMemory(getHL());
            return 8;
        }
        case 0x47: {    // LD B, A: Load A to B
            B = A;
            return 4;
        }
        case 0x40: {    // LD B, B: Load B to B
            return 4;
        }
        case 0x41: {    // LD B, C: Load C to B
            B = C;
            return 4;
        }
        case 0x42: {    // LD B, D: Load D to B
            B = D;
            return 4;
        }
        case 0x43: {    // LD B, E: Load E to B
            B = E;
            return 4;
        }
        case 0x44: {    // LD B, H: Load H to B
            B = H;
            return 4;
        }
        case 0x45: {    // LD B, L: Load L to B
            B = L;
            return 4;
        }
        case 0x46: {    // LD B, (HL): Load memory[HL] to B
            B = readByteFromMemory(getHL());
            return 8;
        }
        case 0x4F: {    // LD C, A: Load A to C
            C = A;
            return 4;
        }
        case 0x48: {    // LD C, B: Load B to C
            C = B;
            return 4;
        }
        case 0x49: {    // LD C, C: Load C to C
            return 4;
        }
        case 0x4A: {    // LD C, D: Load D to C
            C = D;
            return 4;
        }
        case 0x4B: {    // LD C, E: Load E to C
            C = E;
            return 4;
        }
        case 0x4C: {    // LD C, H: Load H to C
            C = H;
            return 4;
        }
        case 0x4D: {    // LD C, L: Load L to C
            C = L;
            return 4;
        }
        case 0x4E: {    // LD C, (HL): Load memory[HL] to C
            C = readByteFromMemory(getHL());
            return 8;
        }
        case 0x57: {    // LD D, A: Load A to D
            D = A;
            return 4;
        }
        case 0x50: {    // LD D, B: Load B to D
            D = B;
            return 4;
        }
        case 0x51: {    // LD D, C: Load C to D
            D = C;
            return 4;
        }
        case 0x52: {    // LD D, D: Load D to D
            return 4;
        }
        case 0x53: {    // LD D, E: Load E to D
            D = E;
            return 4;
        }
        case 0x54: {    // LD D, H: Load H to D
            D = H;
            return 4;
        }
        case 0x55: {    // LD D, L: Load L to D
            D = L;
            return 4;
        }
        case 0x56: {    // LD D, (HL): Load memory[HL] to D
            D = readByteFromMemory(getHL());
            return 8;
        }
        case 0x5F: {    // LD E, A: Load A to E
            E = A;
            return 4;
        }
        case 0x58: {    // LD E, B: Load B to E
            E = B;
            return 4;
        }
        case 0x59: {    // LD E, C: Load C to E
            E = C;
            return 4;
        }
        case 0x5A: {    // LD E, D: Load D to E
            E = D;
            return 4;
        }
        case 0x5B: {    // LD E, E: Load E to E
            return 4;
        }
        case 0x5C: {    // LD E, H: Load H to E
            E = H;
            return 4;
        }
        case 0x5D: {    // LD E, L: Load L to E
            E = L;
            return 4;
        }
        case 0x5E: {    // LD E, (HL): Load memory[HL] to E
            E = readByteFromMemory(getHL());
            return 8;
        }
        case 0x67: {    // LD H, A: Load A to H
            H = A;
            return 4;
        }
        case 0x60: {    // LD H, B: Load B to H
            H = B;
            return 4;
        }
        case 0x61: {    // LD H, C: Load C to H
            H = C;
            return 4;
        }
        case 0x62: {    // LD H, D: Load D to H
            H = D;
            return 4;
        }
        case 0x63: {    // LD H, E: Load E to H
            H = E;
            return 4;
        }
        case 0x64: {    // LD H, H: Load H to H
            return 4;
        }
        case 0x65: {    // LD H, L: Load L to H
            H = L;
            return 4;
        }
        case 0x66: {    // LD H, (HL): Load memory[HL] to H
            H = readByteFromMemory(getHL());
            return 8;
        }
        case 0x6F: {    // LD L, A: Load A to L
            L = A;
            return 4;
        }
        case 0x68: {    // LD L, B: Load B to L
            L = B;
            return 4;
        }
        case 0x69: {    // LD L, C: Load C to L
            L = C;
            return 4;
        }
        case 0x6A: {    // LD L, D: Load D to L
            L = D;
            return 4;
        }
        case 0x6B: {    // LD L, E: Load E to L
            L = E;
            return 4;
        }
        case 0x6C: {    // LD L, H: Load H to L
            L = H;
            return 4;
        }
        case 0x6D: {    // LD L, L: Load L to L
            return 4;
        }
        case 0x6E: {    // LD L, (HL): Load memory[HL] to L
            L = readByteFromMemory(getHL());
            return 8;
        }
        case 0x02: {    // LD (BC), A: Write A to memory[BC]
            writeByteToMemory(getBC(), A);
            return 8;
        }
        case 0x12: {    // LD (DE), A: write A to memory[DE]
            writeByteToMemory(getDE(), A);
            return 8;
        }
        case 0xEA: {    // LD (nn), A: write A to memory[nn]. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            writeByteToMemory(nn, A);
            return 16;
        }
        case 0x77: {    // LD (HL), A: Write A to memory[HL]
            writeByteToMemory(getHL(), A);
            return 8;
        }
        case 0x70: {    // LD (HL), B: Write B to memory[HL]
            writeByteToMemory(getHL(), B);
            return 8;
        }
        case 0x71: {    // LD (HL), C: Write C to memory[HL]
            writeByteToMemory(getHL(), C);
            return 8;
        }
        case 0x72: {    // LD (HL), D: Write D to memory[HL]
            writeByteToMemory(getHL(), D);
            return 8;
        }
        case 0x73: {    // LD (HL), E: Write E to memory[HL]
            writeByteToMemory(getHL(), E);
            return 8;
        }
        case 0x74: {    // LD (HL), H: Write H to memory[HL]
            writeByteToMemory(getHL(), H);
            return 8;
        }
        case 0x75: {    // LD (HL), L: Write L to memory[HL]
            writeByteToMemory(getHL(), L);
            return 8;
        }
        case 0x36: {    // LD (HL), n: Write n to memory[HL]. n = unsigned int 8 bit
            writeByteToMemory(getHL(), readByteFromMemory(pc++));
            return 12;
        }
        case 0x0A: {    // LD A, (BC): Load memory[BC] to A
            A = readByteFromMemory(getBC());
            return 8;
        }
        case 0x1A: {    // LD A, (DE): Load memory[BC] to A
            A = readByteFromMemory(getDE());
            return 8;
        }
        case 0xFA: {    // LD A, (nn): Load memory[nn] to A. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            A = readByteFromMemory(nn);
            return 16;
        }
        case 0x3E: {    // LD A, n: Load n to A. n = unsigned int 8 bit
            A = readByteFromMemory(pc++);
            return 8;
        }
        case 0xF2: {    // LD A, (0xFF00 + C): Load memory[0xFF00 + C] to A
            A = readByteFromMemory(0xFF00 + C);
            return 8;
        }
        case 0xE2: {    // LD (0xFF00 + C), A: Write A to memory[0xFF00 + C]
            writeByteToMemory(0xFF00 + C, A);
        }
        case 0x2A: {    // LDI A, (HL): Load memory[HL] to A and increment HL
            A = readByteFromMemory(getHL());
            setHL(getHL() + 1);
            return 8;
        }
        case 0x3A: {    // LDD A, (HL): Load memory[HL] to A and decrement HL
            A = readByteFromMemory(getHL());
            setHL(getHL() - 1);
            return 8;
        }
        case 0x22: {    // LDI (HL), A: Write A to memory[HL] and increment HL
            writeByteToMemory(getHL(), A);
            setHL(getHL() - 1);
            return 8;
        }
        case 0x32: {    // LDD (HL), A: Write A to memory[HL] and decrement HL
            writeByteToMemory(getHL(), A);
            setHL(getHL() - 1);
            return 8;
        }
        case 0xC3: {    // JP nn: Jump to the absolute address specified by the operand nn. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            pc = (msb << 8) | lsb;
            return 16;
        }
        case 0xE9: {    // JP HL: Jump to the absolute address specified by the register HL
            pc = getHL();
            return 4;
        }
        case 0xC2:      // JP NZ, nn: Jump to nn if zero flag is false.  nn is always read.  nn = unsigned int 16 bit
        case 0xCA:      // JP Z,  nn: Jump to nn if zero flag is true.   nn is always read.  nn = unsigned int 16 bit
        case 0xD2:      // JP NC, nn: Jump to nn if carry flag is false. nn is always read.  nn = unsigned int 16 bit
        case 0xDA: {    // JP C,  nn: Jump to nn if carry flag is true.  nn is always read.  nn = unsigned int 16 bit
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            if ((opcode == 0xC2 && !FZ) || (opcode == 0xCA && FZ) || (opcode == 0xD2 && !FC) || (opcode == 0xDA && FC)) {
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
            if ((opcode == 0x20 && !FZ) || (opcode == 0x28 && FZ) || (opcode == 0x30 && !FC) || (opcode == 0x38 && FC)) {
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
            if ((opcode == 0xC4 && !FZ) || (opcode == 0xCC && FZ) || (opcode == 0xD4 && !FC) || (opcode == 0xDC && FC)) {
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
            if ((opcode == 0xC0 && !FZ) || (opcode == 0xC8 && FZ) || (opcode == 0xD0 && !FC) || (opcode == 0xD8 && FC)) {
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
