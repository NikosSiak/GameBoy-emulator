//
// Created by nikos on 29-Jul-19.
//

#include <CPU/CPU.hpp>

uint8_t CPU::getF() {   // Return a unsigned 8 bit int which represents the F register
    uint8_t regF = 0x00;    // F register: FZ FN FH FC 0 0 0 0
    if (FC) {
        regF += 16;     // 2 ^ 4 (4th bit of register)
    }
    if (FH) {
        regF += 32;     // 2 ^ 5 (5th bit of register)
    }
    if (FN) {
        regF += 64;     // 2 ^ 6 (6th bit of register)
    }
    if (FZ) {
        regF += 128;    // 2 ^ 7 (7th bit of register)
    }
    return regF;
}

void CPU::setF(uint8_t value) {
    FZ = ((value >> 7) & 0x01) == 1;    // If the most significant bit is 1 fz = true else false
    FN = ((value >> 6) & 0x01) == 1;    // If the second most significant bit is 1 fn = true else false
    FH = ((value >> 5) & 0x01) == 1;    // If the third most significant bit is 1 fn = true else false
    FC = ((value >> 4) & 0x01) == 1;    // If the fourth most significant bit is 1 fn = true else false
}

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
    return m_bus.readByteFromMemory(address);
}

void CPU::writeByteToMemory(uint16_t address, uint8_t value) {
    m_bus.writeByteToMemory(address, value);
}

void CPU::enableInterruptRegister() {
    // TODO
}

void CPU::addInstruction(uint8_t target) {      // source register is always A
    FH = (target & 0x0F) > 0xF - (A & 0x0F);
    FC = target > 0xFF - A;
    A += target;
    FZ = A == 0;
    FN = false;
}

void CPU::subInstruction(uint8_t target) {      // source register is always A
    FH = (target & 0x0F) > (A & 0x0F);
    FC = target > A;
    A -= target;
    FZ = A == 0;
    FN = true;
}

void CPU::andInstruction(uint8_t target) {      // source register is always A
    A &= target;
    FZ = A == 0;
    FN = false;
    FH = true;
    FC = false;
}

void CPU::orInstruction(uint8_t target) {       // source register is always A
    A |= target;
    FZ = A == 0;
    FN = false;
    FH = false;
    FC = false;
}

void CPU::xorInstruction(uint8_t target) {      // source register is always A
    A ^= target;
    FZ = A == 0;
    FN = false;
    FH = false;
    FC = false;
}

void CPU::cpInstruction(uint8_t target) {       // source register is always A
    FZ = A == target;   // set if A - target == 0
    FN = true;
    FH = (target & 0x0F) > (A & 0x0F);
    FC = target > A;
}

void CPU::incInstruction(uint8_t &target) {
    FH = 0xF - (target & 0x0F) == 0;
    target++;
    FZ = target == 0;
    FN = false;
    // FC is unmodified
}

void CPU::decInstruction(uint8_t &target) {
    FH = (A & 0x0F) == 0;
    target--;
    FZ = target == 0;
    FN = true;
    // FC is unmodified
}

void CPU::addHLInstruction(uint16_t target) {
    uint16_t hl = getHL();
    FH = (target & 0x0FFF) > 0xFFF - (hl & 0x0FFF);
    FC = target > 0xFFFF - hl;
    setHL(hl + target);
    FN = false;
    // FZ is unmodified
}

void CPU::swapInstruction(uint8_t &target) {
    uint8_t upperNibble = (target & 0xF0) >> 4;
    uint8_t lowerNibble = (target & 0x0F) << 4;
    target = lowerNibble | upperNibble;
    FZ = target == 0;
}

void CPU::rlcInstruction(uint8_t &target) {
    uint8_t bit7 = (target & 0x80) >> 7;
    target <<= 1;
    target |= bit7;
    FC = bit7 == 1;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::rlInstruction(uint8_t &target) {
    uint8_t bit7 = (target & 0x80) >> 7;
    uint8_t bit0 = FC ? 1 : 0;
    target <<= 1;
    target |= bit0;
    FC = bit7 == 1;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::rrcInstruction(uint8_t &target) {
    uint8_t bit0 = (target & 0x01) << 7;
    target >>= 1;
    target |= bit0;
    FC = bit0 != 0;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::rrInstruction(uint8_t &target) {
    uint8_t bit0 = target & 0x01;
    uint8_t bit7 = (FC ? 1 : 0) << 7;
    target >>= 1;
    target |= bit7;
    FC = bit0 == 1;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::slaInstruction(uint8_t &target) {
    uint8_t bit7 = (target & 0x80) >> 7;
    target <<= 1;
    FC = bit7 != 0;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::sraInstruction(uint8_t &target) {
    uint8_t bit0 = target & 0x01;
    uint8_t bit7 = target & 0x80;
    target >>= 1;
    target |= bit7;
    FC = bit0 != 0;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::srlInstruction(uint8_t &target) {
    uint8_t bit0 = target & 0x01;
    target >>= 1;
    FC = bit0 != 0;
    FZ = target == 0;
    FN = false;
    FH = false;
}

void CPU::bitInstruction(uint8_t target, int bit) {
    FZ = ((target >> bit) & 0x01) == 0;
    FN = false;
    FH = true;
}

void CPU::setInstruction(uint8_t &target, int bit) {
    uint8_t setbit = 1 << bit;
    target |= setbit;
}

void CPU::resInstruction(uint8_t &target, int bit) {
    uint8_t lowerbits = target << (8 - bit);
    lowerbits >>= (8 - bit);
    target >>= (bit + 1);
    target <<= (bit + 1);
    target |= lowerbits;
}

int CPU::emulateInstruction() {     // returns number of cycles needed for the instruction

    uint8_t opcode = readByteFromMemory(pc++);

    switch (opcode) {
        case 0x00: {    // NOP: No operation
            return 4;
        }
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
        case 0xE0: {    // LD (0xFF00 + n), A: Write A to memory[0xFF00 + n]. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            writeByteToMemory(0xFF00 + n, A);
            return 12;
        }
        case 0xF0: {    // LD A, (0xFF00 + n): Load memory[0xFF00 + n] to A. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            A = readByteFromMemory(0xFF00 + n);
        }
        case 0x01: {    // LD BC, nn: Load nn to BC. nn = unsigned int 16 bit. LSB first
            C = readByteFromMemory(pc++);
            B = readByteFromMemory(pc++);
            return 12;
        }
        case 0x11: {    // LD DE, nn: Load nn to DE. nn = unsigned int 16 bit. LSB first
            E = readByteFromMemory(pc++);
            D = readByteFromMemory(pc++);
            return 12;
        }
        case 0x21: {    // LD HL, nn: Load nn to HL. nn = unsigned int 16 bit. LSB first
            L = readByteFromMemory(pc++);
            H = readByteFromMemory(pc++);
            return 12;
        }
        case 0x31: {    // LD SP, nn: Load nn to SP. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            sp = (msb << 8) | lsb;
            return 12;
        }
        case 0xF9: {    // LD SP, HL: Load HL to SP
            sp = getHL();
            return 8;
        }
        case 0xF8: {    // LD HL, SP + n. Load SP + n to HL. n = signed int 8 bit
            int8_t n = static_cast<int8_t>(readByteFromMemory(pc++));
            setHL(sp + n);
            FZ = false;
            FN = false;
            FH = (n & 0x0F) > 0xF - (sp & 0x000F);
            FC = n > 0xFFFF - sp;
            return 12;
        }
        case 0x08: {    // LD (nn), SP: Write SP to memory[nn]. nn = unsigned int 16 bit. LSB first
            uint8_t lsb = readByteFromMemory(pc++);
            uint8_t msb = readByteFromMemory(pc++);
            uint16_t nn = (msb << 8) | lsb;
            writeByteToMemory(nn, sp >> 8);
            writeByteToMemory(nn + 1, pc & 0x00FF);
            return 20;
        }
        case 0xF5: {    // PUSH AF: Write AF to stack
            writeByteToMemory(--sp, A);
            writeByteToMemory(--sp, getF());
            return 16;
        }
        case 0xC5: {    // PUSH BC: Write BC to stack
            writeByteToMemory(--sp, B);
            writeByteToMemory(--sp, C);
            return 16;
        }
        case 0xD5: {    // PUSH DE: Write DE to stack
            writeByteToMemory(--sp, D);
            writeByteToMemory(--sp, E);
            return 16;
        }
        case 0xE5: {    // PUSH HL: Write HL to stack
            writeByteToMemory(--sp, H);
            writeByteToMemory(--sp, L);
            return 16;
        }
        case 0xF1: {    // POP AF: Load AF from stack
            setF(readByteFromMemory(sp++));
            A = readByteFromMemory(sp++);
            return 12;
        }
        case 0xC1: {    // POP BC: Load BC from stack
            C = readByteFromMemory(sp++);
            B = readByteFromMemory(sp++);
            return 12;
        }
        case 0xD1: {    // POP DE: Load DE from stack
            E = readByteFromMemory(sp++);
            D = readByteFromMemory(sp++);
            return 12;
        }
        case 0xE1: {    // POP HL: Load HL from stack
            L = readByteFromMemory(sp++);
            H = readByteFromMemory(sp++);
            return 12;
        }
        case 0x87: {    // ADD A, A: Adds A to A
            addInstruction(A);
            return 4;
        }
        case 0x80: {    // ADD A, B: Adds B to A
            addInstruction(B);
            return 4;
        }
        case 0x81: {    // ADD A, C: Adds C to A
            addInstruction(C);
            return 4;
        }
        case 0x82: {    // ADD A, D: Adds D to A
            addInstruction(D);
            return 4;
        }
        case 0x83: {    // ADD A, E: Adds E to A
            addInstruction(E);
            return 4;
        }
        case 0x84: {    // ADD A, H: Adds H to A
            addInstruction(H);
            return 4;
        }
        case 0x85: {    // ADD A, L: Adds L to A
            addInstruction(L);
            return 4;
        }
        case 0x86: {    // ADD A, (HL): Adds memory[HL] to A
            uint8_t n = readByteFromMemory(getHL());
            addInstruction(n);
            return 8;
        }
        case 0xC6: {    // ADD A, n: Adds n to A. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            addInstruction(n);
            return 8;
        }
        case 0x8F: {    // ADC A, A: Adds A + carry flag to A
            addInstruction(A + (FC ? 1 : 0));
            return 4;
        }
        case 0x88: {    // ADC A, B: Adds B + carry flag to A
            addInstruction(B + (FC ? 1 : 0));
            return 4;
        }
        case 0x89: {    // ADC A, C: Adds C + carry flag to A
            addInstruction(C + (FC ? 1 : 0));
            return 4;
        }
        case 0x8A: {    // ADC A, D: Adds D + carry flag to A
            addInstruction(D + (FC ? 1 : 0));
            return 4;
        }
        case 0x8B: {    // ADC A, E: Adds E + carry flag to A
            addInstruction(E + (FC ? 1 : 0));
            return 4;
        }
        case 0x8C: {    // ADC A, H: Adds H + carry flag to A
            addInstruction(H + (FC ? 1 : 0));
            return 4;
        }
        case 0x8D: {    // ADC A, L: Adds L + carry flag to A
            addInstruction(L + (FC ? 1 : 0));
            return 4;
        }
        case 0x8E: {    // ADC A, (HL): Adds memory[HL} + carry flag to A
            uint8_t n = readByteFromMemory(getHL());
            addInstruction(n + (FC ? 1 : 0));
            return 8;
        }
        case 0xCE: {    // ADC A, n: Adds n + carry flag to A. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            addInstruction(n + (FC ? 1 : 0));
            return 8;
        }
        case 0x97: {    // SUB A, A: Subtracts A from A
            subInstruction(A);
            return 4;
        }
        case 0x90: {    // SUB A, B: Subtracts B from A
            subInstruction(B);
            return 4;
        }
        case 0x91: {    // SUB A, C: Subtracts C from A
            subInstruction(C);
            return 4;
        }
        case 0x92: {    // SUB A, D: Subtracts D from A
            subInstruction(D);
            return 4;
        }
        case 0x93: {    // SUB A, E: Subtracts E from A
            subInstruction(E);
            return 4;
        }
        case 0x94: {    // SUB A, H: Subtracts H from A
            subInstruction(H);
            return 4;
        }
        case 0x95: {    // SUB A, L: Subtracts L from A
            subInstruction(L);
            return 4;
        }
        case 0x96: {    // SUB A, (HL): Subtracts memory[HL] from A
            uint8_t n = readByteFromMemory(getHL());
            subInstruction(n);
            return 8;
        }
        case 0xD6: {    // SUB A, n: Subtracts n from A. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            subInstruction(n);
            return 8;
        }
        case 0x9F: {    // SBC A, A: Subtracts A + carry flag from A
            subInstruction(A + (FC ? 1 : 0));
            return 4;
        }
        case 0x98: {    // SBC A, B: Subtracts B + carry flag from A
            subInstruction(B + (FC ? 1 : 0));
            return 4;
        }
        case 0x99: {    // SBC A, C: Subtracts C + carry flag from A
            subInstruction(C + (FC ? 1 : 0));
            return 4;
        }
        case 0x9A: {    // SBC A, D: Subtracts D + carry flag from A
            subInstruction(D + (FC ? 1 : 0));
            return 4;
        }
        case 0x9B: {    // SBC A, E: Subtracts E + carry flag from A
            subInstruction(E + (FC ? 1 : 0));
            return 4;
        }
        case 0x9C: {    // SBC A, H: Subtracts H + carry flag from A
            subInstruction(H + (FC ? 1 : 0));
            return 4;
        }
        case 0x9D: {    // SBC A, L: Subtracts L + carry flag from A
            subInstruction(L + (FC ? 1 : 0));
            return 4;
        }
        case 0x9E: {    // SBC A, (HL): Subtracts memory[HL] + carry flag from A
            uint8_t n = readByteFromMemory(getHL());
            subInstruction(n + (FC ? 1 : 0));
            return 8;
        }
        case 0xDE: {    // SBC A, n: Subtracts n + carry flag from A. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            subInstruction(n + (FC ? 1 : 0));
            return 8;
        }
        case 0xA7: {    // AND A, A: Set A to bitwise and between A and A
            andInstruction(A);
            return 4;
        }
        case 0xA0: {    // AND A, B: Set A to bitwise and between A and B
            andInstruction(B);
            return 4;
        }
        case 0xA1: {    // AND A, C: Set A to bitwise and between A and C
            andInstruction(C);
            return 4;
        }
        case 0xA2: {    // AND A, D: Set A to bitwise and between A and D
            andInstruction(D);
            return 4;
        }
        case 0xA3: {    // AND A, E: Set A to bitwise and between A and E
            andInstruction(A);
            return 4;
        }
        case 0xA4: {    // AND A, H: Set A to bitwise and between A and H
            andInstruction(H);
            return 4;
        }
        case 0xA5: {    // AND A, L: Set A to bitwise and between A and L
            andInstruction(L);
            return 4;
        }
        case 0xA6: {    // AND A, (HL): Set A to bitwise and between A and memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            andInstruction(n);
            return 8;
        }
        case 0xE6: {    // AND A, n: Set A to bitwise and between A and n. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            andInstruction(n);
            return 8;
        }
        case 0xB7: {    // OR A, A: Set A to bitwise or between A and A
            orInstruction(A);
            return 4;
        }
        case 0xB0: {    // OR A, B: Set A to bitwise or between A and B
            orInstruction(B);
            return 4;
        }
        case 0xB1: {    // OR A, C: Set A to bitwise or between A and C
            orInstruction(C);
            return 4;
        }
        case 0xB2: {    // OR A, D: Set A to bitwise or between A and D
            orInstruction(D);
            return 4;
        }
        case 0xB3: {    // OR A, E: Set A to bitwise or between A and E
            orInstruction(E);
            return 4;
        }
        case 0xB4: {    // OR A, H: Set A to bitwise or between A and H
            orInstruction(H);
            return 4;
        }
        case 0xB5: {    // OR A, L: Set A to bitwise or between A and L
            orInstruction(L);
            return 4;
        }
        case 0xB6: {    // OR A, (HL): Set A to bitwise or between A and memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            orInstruction(n);
            return 8;
        }
        case 0xF6: {    // OR A, n: Set A to bitwise or between A and n. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            orInstruction(n);
            return 8;
        }
        case 0xAF: {    // XOR A, A: Set A to bitwise xor between A and A
            xorInstruction(A);
            return 4;
        }
        case 0xA8: {    // XOR A, B: Set A to bitwise xor between A and B
            xorInstruction(B);
            return 4;
        }
        case 0xA9: {    // XOR A, C: Set A to bitwise xor between A and C
            xorInstruction(C);
            return 4;
        }
        case 0xAA: {    // XOR A, D: Set A to bitwise xor between A and D
            xorInstruction(D);
            return 4;
        }
        case 0xAB: {    // XOR A, E: Set A to bitwise xor between A and E
            xorInstruction(E);
            return 4;
        }
        case 0xAC: {    // XOR A, H: Set A to bitwise xor between A and H
            xorInstruction(H);
            return 4;
        }
        case 0xAD: {    // XOR A, L: Set A to bitwise xor between A and L
            xorInstruction(L);
            return 4;
        }
        case 0xAE: {    // XOR A, (HL): Set A to bitwise xor between A and memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            xorInstruction(n);
            return 8;
        }
        case 0xEE: {    // XOR A, n: Set A to bitwise xor between A and n. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            xorInstruction(n);
            return 8;
        }
        case 0xBF: {    // CP A, A: Compare A with A
            cpInstruction(A);
            return 4;
        }
        case 0xB8: {    // CP A, B: Compare A with B
            cpInstruction(B);
            return 4;
        }
        case 0xB9: {    // CP A, C: Compare A with C
            cpInstruction(C);
            return 4;
        }
        case 0xBA: {    // CP A, D: Compare A with D
            cpInstruction(D);
            return 4;
        }
        case 0xBB: {    // CP A, E: Compare A with E
            cpInstruction(E);
            return 4;
        }
        case 0xBC: {    // CP A, H: Compare A with H
            cpInstruction(H);
            return 4;
        }
        case 0xBD: {    // CP A, L: Compare A with L
            cpInstruction(L);
            return 4;
        }
        case 0xBE: {    // CP A, (HL): Compare A with memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            cpInstruction(n);
            return 8;
        }
        case 0xFE: {    // CP A, n: Compare A with n. n = unsigned int 8 bit
            uint8_t n = readByteFromMemory(pc++);
            cpInstruction(n);
            return 8;
        }
        case 0x3C: {    // INC A: Increment A
            incInstruction(A);
            return 4;
        }
        case 0x04: {    // INC B: Increment B
            incInstruction(B);
            return 4;
        }
        case 0x0C: {    // INC C: Increment C
            incInstruction(C);
            return 4;
        }
        case 0x14: {    // INC D: Increment D
            incInstruction(D);
            return 4;
        }
        case 0x1C: {    // INC E: Increment E
            incInstruction(E);
            return 4;
        }
        case 0x24: {    // INC H: Increment H
            incInstruction(H);
            return 4;
        }
        case 0x2C: {    // INC L: Increment L
            incInstruction(L);
            return 4;
        }
        case 0x34: {    // INC (HL): Increment memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            incInstruction(n);
            writeByteToMemory(getHL(), n);
            return 12;
        }
        case 0x3D: {    // DEC A: Decrement A
            decInstruction(A);
            return 4;
        }
        case 0x05: {    // DEC B: Decrement B
            decInstruction(B);
            return 4;
        }
        case 0x0D: {    // DEC C: Decrement C
            decInstruction(C);
            return 4;
        }
        case 0x15: {    // DEC D: Decrement D
            decInstruction(D);
            return 4;
        }
        case 0x1D: {    // DEC E: Decrement E
            decInstruction(E);
            return 4;
        }
        case 0x25: {    // DEC H: Decrement H
            decInstruction(H);
            return 4;
        }
        case 0x2D: {    // DEC L: Decrement L
            decInstruction(L);
            return 4;
        }
        case 0x35: {    // DEC (HL): Decrement memory[HL]
            uint8_t n = readByteFromMemory(getHL());
            decInstruction(n);
            writeByteToMemory(getHL(), n);
            return 12;
        }
        case 0xE8: {    // ADD SP, n: Adds n to SP. n = signed int 8 bit
            int8_t n = static_cast<int8_t>(readByteFromMemory(pc++));
            FZ = false;
            FN = false;
            if (n < 0) {
                FH = (n & 0x0F) > (sp & 0x000F);
                FC = false;
            }
            else {
                FH = (n & 0x0F) > 0xF - (sp & 0x000F);
                FC = n > 0xFFFF - sp;
            }
            sp += n;
            return 16;
        }
        case 0x09: {    // ADD HL, BC: Adds BC to HL
            addHLInstruction(getBC());
            return 8;
        }
        case 0x19: {    // ADD HL, DE: Adds BC to DE
            addHLInstruction(getDE());
            return 8;
        }
        case 0x29: {    // ADD HL, HL: Adds HL to HL
            addHLInstruction(getHL());
            return 8;
        }
        case 0x39: {    // ADD HL, SP: Adds SP to HL
            addHLInstruction(sp);
            return 8;
        }
        case 0x03: {    // INC BC: Increments BC
            setBC(getBC() + 1);
            return 8;
        }
        case 0x13: {    // INC DE: Increments DE
            setDE(getDE() + 1);
            return 8;
        }
        case 0x23: {    // INC HL: Increments HL
            setHL(getHL() + 1);
            return 8;
        }
        case 0x33: {    // INC SP: Increments SP
            sp++;
            return 8;
        }
        case 0x0B: {    // DEC BC: Decrements BC
            setBC(getBC() - 1);
            return 8;
        }
        case 0x1B: {    // DEC DE: Decrements DE
            setDE(getDE() - 1);
            return 8;
        }
        case 0x2B: {    // DEC HL: Decrements HL
            setHL(getHL() - 1);
            return 8;
        }
        case 0x3B: {    // DEC SP: Decrements SP
            sp--;
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
        case 0xC7: {    // RST 0x00: Puh pc to stack and jump to address 0x0000
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0000;
            return 16;
        }
        case 0xCF: {    // RST 0x08: Puh pc to stack and jump to address 0x0008
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0008;
            return 16;
        }
        case 0xD7: {    // RST 0x10: Puh pc to stack and jump to address 0x0010
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0010;
            return 16;
        }
        case 0xDF: {    // RST 0x18: Puh pc to stack and jump to address 0x0018
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0018;
            return 16;
        }
        case 0xE7: {    // RST 0x20: Puh pc to stack and jump to address 0x0020
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0020;
            return 16;
        }
        case 0xEF: {    // RST 0x28: Puh pc to stack and jump to address 0x0028
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0028;
            return 16;
        }
        case 0xF7: {    // RST 0x30: Puh pc to stack and jump to address 0x0030
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0030;
            return 16;
        }
        case 0xFF: {    // RST 0x38: Puh pc to stack and jump to address 0x0038
            writeByteToMemory(--sp, pc >> 8);       // write msb of pc to stack
            writeByteToMemory(--sp, pc & 0x00FF);   // write lsb of pc to stack
            pc = 0x0038;
            return 16;
        }
        case 0x27: {    // DAA: Decimal adjust register A
            uint8_t adjust = 0;
            if ((A  > 0x99 && !FN) || FC) {
                adjust |= 0x60;
                FC = true;
            }
            else {
                FC = false;
            }
            if (((A & 0x0F) > 0x09 && !FN) || FH) {
                adjust |= 0x06;
            }
            A += FN ? -adjust : adjust;
            FZ = A == 0;
            FH = false;
            return 4;
        }
        case 0x2F: {    // CPL: Complement register A
            A = ~A;
            FN = true;
            FH = true;
            return 4;
        }
        case 0x37: {    // SCF: Set Carry flag
            FC = true;
            FN = false;
            FH = false;
            return 4;
        }
        case 0x3F: {    //  CCF: Complement carry flag
            FC = !FC;
            FN = false;
            FH = false;
            return 4;
        }
        case 0x76: {    // HALT: Power down CPU until an interrupt occurs
            // TODO
            return 4;
        }
        case 0x10: {    // STOP: Halt CPU & LCD display until button pressed. 2 bytes
            readByteFromMemory(pc++);
            // TODO
            return 4;
        }
        case 0xF3: {    // DI: Disables interrupts
            // TODO
            return 4;
        }
        case 0xFB: {    // EI: Enable interrupts
            // TODO
            return 4;
        }
        case 0x07: {    // RLCA: Rotate A left. Old bit 7 to Carry flag. Resets zero flag
            rlcInstruction(A);
            FZ = false;
            return 4;
        }
        case 0x17: {    //  RLA: Rotate A left through Carry flag. Resets zero flag
            rlInstruction(A);
            FZ = false;
            return 4;
        }
        case 0x0F: {    // RRCA: Rotate A right. Old bit 0 to Carry flag. Resets zero flag
            rrcInstruction(A);
            FZ = false;
            return 4;
        }
        case 0x1F: {    // RRA: Rotate A right through Carry flag. Resets zero flag
            rrInstruction(A);
            FZ = false;
            return 4;
        }
        case 0xCB: {    // Prefix CB
            uint8_t opcodeSecondByte = readByteFromMemory(pc++);
            switch (opcodeSecondByte) {
                case 0x37: {    // SWAP A: Swap upper & lower nibbles of A
                    swapInstruction(A);
                    return 8;
                }
                case 0x30: {    // SWAP B: Swap upper & lower nibbles of B
                    swapInstruction(B);
                    return 8;
                }
                case 0x31: {    // SWAP C: Swap upper & lower nibbles of C
                    swapInstruction(C);
                    return 8;
                }
                case 0x32: {    // SWAP D: Swap upper & lower nibbles of D
                    swapInstruction(A);
                    return 8;
                }
                case 0x33: {    // SWAP E: Swap upper & lower nibbles of E
                    swapInstruction(E);
                    return 8;
                }
                case 0x34: {    // SWAP H: Swap upper & lower nibbles of H
                    swapInstruction(H);
                    return 8;
                }
                case 0x35: {    // SWAP L: Swap upper & lower nibbles of L
                    swapInstruction(L);
                    return 8;
                }
                case 0x36: {    // SWAP (HL): Swap upper & lower nibbles of memory[HL]
                    uint8_t n = readByteFromMemory(getHL());
                    swapInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x07: {    // RLC A: Rotate A left. Old bit 7 to Carry flag
                    rlcInstruction(A);
                    return 8;
                }
                case 0x00: {    // RLC B: Rotate B left. Old bit 7 to Carry flag
                    rlcInstruction(B);
                    return 8;
                }
                case 0x01: {    // RLC C: Rotate C left. Old bit 7 to Carry flag
                    rlcInstruction(C);
                    return 8;
                }
                case 0x02: {    // RLC D: Rotate D left. Old bit 7 to Carry flag
                    rlcInstruction(D);
                    return 8;
                }
                case 0x03: {    // RLC E: Rotate E left. Old bit 7 to Carry flag
                    rlcInstruction(E);
                    return 8;
                }
                case 0x04: {    // RLC H: Rotate H left. Old bit 7 to Carry flag
                    rlcInstruction(H);
                    return 8;
                }
                case 0x05: {    // RLC L: Rotate L left. Old bit 7 to Carry flag
                    rlcInstruction(L);
                    return 8;
                }
                case 0x06: {    // RLC (HL): Rotate memory[HL] left. Old bit 7 to Carry flag
                    uint8_t n = readByteFromMemory(getHL());
                    rlcInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x17: {    // RL A: Rotate A left through Carry flag
                    rlInstruction(A);
                    return 8;
                }
                case 0x10: {    // RL B: Rotate B left through Carry flag
                    rlInstruction(B);
                    return 8;
                }
                case 0x11: {    // RL C: Rotate C left through Carry flag
                    rlInstruction(C);
                    return 8;
                }
                case 0x12: {    // RL D: Rotate D left through Carry flag
                    rlInstruction(D);
                    return 8;
                }
                case 0x13: {    // RL E: Rotate E left through Carry flag
                    rlInstruction(E);
                    return 8;
                }
                case 0x14: {    // RL H: Rotate H left through Carry flag
                    rlInstruction(H);
                    return 8;
                }
                case 0x15: {    // RL L: Rotate L left through Carry flag
                    rlInstruction(L);
                    return 8;
                }
                case 0x16: {    // RL (HL): Rotate memory[HL] left through Carry flag
                    uint8_t n = readByteFromMemory(getHL());
                    rlInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x0F: {    // RRC A: Rotate A right. Old bit 0 to Carry flag
                    rrcInstruction(A);
                    return 8;
                }
                case 0x08: {    // RRC B: Rotate B right. Old bit 0 to Carry flag
                    rrcInstruction(B);
                    return 8;
                }
                case 0x09: {    // RRC C: Rotate C right. Old bit 0 to Carry flag
                    rrcInstruction(C);
                    return 8;
                }
                case 0x0A: {    // RRC D: Rotate D right. Old bit 0 to Carry flag
                    rrcInstruction(D);
                    return 8;
                }
                case 0x0B: {    // RRC E: Rotate E right. Old bit 0 to Carry flag
                    rrcInstruction(E);
                    return 8;
                }
                case 0x0C: {    // RRC H: Rotate H right. Old bit 0 to Carry flag
                    rrcInstruction(H);
                    return 8;
                }
                case 0x0D: {    // RRC L: Rotate L right. Old bit 0 to Carry flag
                    rrcInstruction(L);
                    return 8;
                }
                case 0x0E: {    // RRC (HL): Rotate memory[HL] right. Old bit 0 to Carry flag
                    uint8_t n = readByteFromMemory(getHL());
                    rrcInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x1F: {    // RR A: Rotate A right through Carry flag
                    rrInstruction(A);
                    return 8;
                }
                case 0x18: {    // RR B: Rotate B right through Carry flag
                    rrInstruction(B);
                    return 8;
                }
                case 0x19: {    // RR C: Rotate C right through Carry flag
                    rrInstruction(C);
                    return 8;
                }
                case 0x1A: {    // RR D: Rotate D right through Carry flag
                    rrInstruction(D);
                    return 8;
                }
                case 0x1B: {    // RR E: Rotate E right through Carry flag
                    rrInstruction(E);
                    return 8;
                }
                case 0x1C: {    // RR H: Rotate H right through Carry flag
                    rrInstruction(H);
                    return 8;
                }
                case 0x1D: {    // RR L: Rotate L right through Carry flag
                    rrInstruction(L);
                    return 8;
                }
                case 0x1E: {    // RR (HL): Rotate memory[HL] right through Carry flag
                    uint8_t n = readByteFromMemory(getHL());
                    rrInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x27: {    // SLA A: Shift A left into Carry
                    slaInstruction(A);
                    return 8;
                }
                case 0x20: {    // SLA B: Shift B left into Carry
                    slaInstruction(B);
                    return 8;
                }
                case 0x21: {    // SLA C: Shift C left into Carry
                    slaInstruction(C);
                    return 8;
                }
                case 0x22: {    // SLA D: Shift D left into Carry
                    slaInstruction(D);
                    return 8;
                }
                case 0x23: {    // SLA E: Shift E left into Carry
                    slaInstruction(E);
                    return 8;
                }
                case 0x24: {    // SLA H: Shift H left into Carry
                    slaInstruction(H);
                    return 8;
                }
                case 0x25: {    // SLA L: Shift L left into Carry
                    slaInstruction(L);
                    return 8;
                }
                case 0x26: {    // SLA (HL): Shift memory[HL] left into Carry
                    uint8_t n = readByteFromMemory(getHL());
                    slaInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x2F: {    // SRA A: Shift A right into Carry. MSB doesn't change
                    sraInstruction(A);
                    return 8;
                }
                case 0x28: {    // SRA B: Shift B right into Carry. MSB doesn't change
                    sraInstruction(B);
                    return 8;
                }
                case 0x29: {    // SRA C: Shift C right into Carry. MSB doesn't change
                    sraInstruction(C);
                    return 8;
                }
                case 0x2A: {    // SRA D: Shift D right into Carry. MSB doesn't change
                    sraInstruction(D);
                    return 8;
                }
                case 0x2B: {    // SRA E: Shift E right into Carry. MSB doesn't change
                    sraInstruction(E);
                    return 8;
                }
                case 0x2C: {    // SRA H: Shift H right into Carry. MSB doesn't change
                    sraInstruction(H);
                    return 8;
                }
                case 0x2D: {    // SRA L: Shift L right into Carry. MSB doesn't change
                    sraInstruction(L);
                    return 8;
                }
                case 0x2E: {    // SRA (HL): Shift memory[HL] right into Carry. MSB doesn't change
                    uint8_t n = readByteFromMemory(getHL());
                    sraInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x3F: {    // SRL A: Shift A right into Carry
                    srlInstruction(A);
                    return 8;
                }
                case 0x38: {    // SRL B: Shift B right into Carry
                    srlInstruction(B);
                    return 8;
                }
                case 0x39: {    // SRL C: Shift C right into Carry
                    srlInstruction(C);
                    return 8;
                }
                case 0x3A: {    // SRL D: Shift D right into Carry
                    srlInstruction(D);
                    return 8;
                }
                case 0x3B: {    // SRL E: Shift E right into Carry
                    srlInstruction(E);
                    return 8;
                }
                case 0x3C: {    // SRL H: Shift H right into Carry
                    srlInstruction(H);
                    return 8;
                }
                case 0x3D: {    // SRL L: Shift L right into Carry
                    srlInstruction(L);
                    return 8;
                }
                case 0x3E: {    // SRL (HL): Shift memory[HL] right into Carry
                    uint8_t n = readByteFromMemory(getHL());
                    srlInstruction(n);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0x47:      // BIT 0, A: Test bit 0 of A
                case 0x4F:      // BIT 1, A: Test bit 1 of A
                case 0x57:      // BIT 2, A: Test bit 2 of A
                case 0x5F:      // BIT 3, A: Test bit 3 of A
                case 0x67:      // BIT 4, A: Test bit 4 of A
                case 0x6F:      // BIT 5, A: Test bit 5 of A
                case 0x77:      // BIT 6, A: Test bit 6 of A
                case 0x7F: {    // BIT 7, A: Test bit 7 of A
                    bitInstruction(A, (opcodeSecondByte - 0x47) >> 3);  // By subtracting the first case value and
                    return 8;                          // shifting right by 3 i get the number of the bit i want to test
                }
                case 0x40:      // BIT n, B: Test bit n of B
                case 0x48:
                case 0x50:
                case 0x58:
                case 0x60:
                case 0x68:
                case 0x70:
                case 0x78: {
                    bitInstruction(B, (opcodeSecondByte - 0x40) >> 3);
                    return 8;
                }
                case 0x41:      // BIT n, C: Test bit n of C
                case 0x49:
                case 0x51:
                case 0x59:
                case 0x61:
                case 0x69:
                case 0x71:
                case 0x79: {
                    bitInstruction(C, (opcodeSecondByte - 0x41) >> 3);
                    return 8;
                }
                case 0x42:      // BIT n, D: Test bit n of D
                case 0x4A:
                case 0x52:
                case 0x5A:
                case 0x62:
                case 0x6A:
                case 0x72:
                case 0x7A: {
                    bitInstruction(D, (opcodeSecondByte - 0x42) >> 3);
                    return 8;
                }
                case 0x43:      // BIT n, E: Test bit n of E
                case 0x4B:
                case 0x53:
                case 0x5B:
                case 0x63:
                case 0x6B:
                case 0x73:
                case 0x7B: {
                    bitInstruction(E, (opcodeSecondByte - 0x43) >> 3);
                    return 8;
                }
                case 0x44:      // BIT n, H: Test bit n of H
                case 0x4C:
                case 0x54:
                case 0x5C:
                case 0x64:
                case 0x6C:
                case 0x74:
                case 0x7C: {
                    bitInstruction(H, (opcodeSecondByte - 0x44) >> 3);
                    return 8;
                }
                case 0x45:      // BIT n, L: Test bit n of L
                case 0x4D:
                case 0x55:
                case 0x5D:
                case 0x65:
                case 0x6D:
                case 0x75:
                case 0x7D: {
                    bitInstruction(L, (opcodeSecondByte - 0x45) >> 3);
                    return 8;
                }
                case 0x46:      // BIT n, (HL): Test bit n of memory[HL]
                case 0x4E:
                case 0x56:
                case 0x5E:
                case 0x66:
                case 0x6E:
                case 0x76:
                case 0x7E: {
                    uint8_t n = readByteFromMemory(getHL());
                    bitInstruction(n, (opcodeSecondByte - 0x46) >> 3);
                    return 12;
                }
                case 0x87:      // RES n, A: Reset bit n of A. n = 0
                case 0x8F:      // n = 1
                case 0x97:      // n = 2
                case 0x9F:      // n = 3
                case 0xA7:      // n = 4
                case 0xAF:      // n = 5
                case 0xB7:      // n = 6
                case 0xBF: {    // n = 7
                    resInstruction(A, (opcodeSecondByte - 0x87) >> 3);
                    return 8;
                }
                case 0x80:      // RES n, B: Reset bit n of B
                case 0x88:
                case 0x90:
                case 0x98:
                case 0xA0:
                case 0xA8:
                case 0xB0:
                case 0xB8: {
                    resInstruction(B, (opcodeSecondByte - 0x80) >> 3);
                    return 8;
                }
                case 0x81:      // RES n, C: Reset bit n of C
                case 0x89:
                case 0x91:
                case 0x99:
                case 0xA1:
                case 0xA9:
                case 0xB1:
                case 0xB9: {
                    resInstruction(C, (opcodeSecondByte - 0x81) >> 3);
                    return 8;
                }
                case 0x82:      // RES n, D: Reset bit n of D
                case 0x8A:
                case 0x92:
                case 0x9A:
                case 0xA2:
                case 0xAA:
                case 0xB2:
                case 0xBA: {
                    resInstruction(D, (opcodeSecondByte - 0x82) >> 3);
                    return 8;
                }
                case 0x83:      // RES n, E: Reset bit n of E
                case 0x8B:
                case 0x93:
                case 0x9B:
                case 0xA3:
                case 0xAB:
                case 0xB3:
                case 0xBB: {
                    resInstruction(E, (opcodeSecondByte - 0x83) >> 3);
                    return 8;
                }
                case 0x84:      // RES n, H: Reset bit n of H
                case 0x8C:
                case 0x94:
                case 0x9C:
                case 0xA4:
                case 0xAC:
                case 0xB4:
                case 0xBC: {
                    resInstruction(H, (opcodeSecondByte - 0x84) >> 3);
                    return 8;
                }
                case 0x85:      // RES n, L: Reset bit n of L
                case 0x8D:
                case 0x95:
                case 0x9D:
                case 0xA5:
                case 0xAD:
                case 0xB5:
                case 0xBD: {
                    resInstruction(L, (opcodeSecondByte - 0x85) >> 3);
                    return 8;
                }
                case 0x86:      // RES n, (HL): Reset bit n of memory[HL]
                case 0x8E:
                case 0x96:
                case 0x9E:
                case 0xA6:
                case 0xAE:
                case 0xB6:
                case 0xBE: {
                    uint8_t n = readByteFromMemory(getHL());
                    resInstruction(n, (opcodeSecondByte - 0x86) >> 3);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }
                case 0xC7:      // SET n, A: Set bit n of A. n = 0
                case 0xCF:      // n = 1
                case 0xD7:      // n = 2
                case 0xDF:      // n = 3
                case 0xE7:      // n = 4
                case 0xEF:      // n = 5
                case 0xF7:      // n = 6
                case 0xFF: {    // n = 7
                    setInstruction(A, (opcodeSecondByte - 0x87) >> 3);
                    return 8;
                }
                case 0xC0:      // SET n, B: Set bit n of B
                case 0xC8:
                case 0xD0:
                case 0xD8:
                case 0xE0:
                case 0xE8:
                case 0xF0:
                case 0xF8: {
                    setInstruction(B, (opcodeSecondByte - 0x80) >> 3);
                    return 8;
                }
                case 0xC1:      // SET n, C: Set bit n of C
                case 0xC9:
                case 0xD1:
                case 0xD9:
                case 0xE1:
                case 0xE9:
                case 0xF1:
                case 0xF9: {
                    setInstruction(C, (opcodeSecondByte - 0x81) >> 3);
                    return 8;
                }
                case 0xC2:      // SET n, D: Set bit n of D
                case 0xCA:
                case 0xD2:
                case 0xDA:
                case 0xE2:
                case 0xEA:
                case 0xF2:
                case 0xFA: {
                    setInstruction(D, (opcodeSecondByte - 0x82) >> 3);
                    return 8;
                }
                case 0xC3:      // SET n, E: Set bit n of E
                case 0xCB:
                case 0xD3:
                case 0xDB:
                case 0xE3:
                case 0xEB:
                case 0xF3:
                case 0xFB: {
                    setInstruction(E, (opcodeSecondByte - 0x83) >> 3);
                    return 8;
                }
                case 0xC4:      // SET n, H: Set bit n of H
                case 0xCC:
                case 0xD4:
                case 0xDC:
                case 0xE4:
                case 0xEC:
                case 0xF4:
                case 0xFC: {
                    setInstruction(H, (opcodeSecondByte - 0x84) >> 3);
                    return 8;
                }
                case 0xC5:      // SET n, L: Set bit n of L
                case 0xCD:
                case 0xD5:
                case 0xDD:
                case 0xE5:
                case 0xED:
                case 0xF5:
                case 0xFD: {
                    setInstruction(L, (opcodeSecondByte - 0x85) >> 3);
                    return 8;
                }
                case 0xC6:      // SET n, (HL): Set bit n of memory[HL]
                case 0xCE:
                case 0xD6:
                case 0xDE:
                case 0xE6:
                case 0xEE:
                case 0xF6:
                case 0xFE: {
                    uint8_t n = readByteFromMemory(getHL());
                    setInstruction(n, (opcodeSecondByte - 0x86) >> 3);
                    writeByteToMemory(getHL(), n);
                    return 16;
                }

            }
        }
        default:
            return 0;
    }
}
