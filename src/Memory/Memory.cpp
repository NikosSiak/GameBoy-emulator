//
// Created by nikos on 29-Jul-19.
//

#include <fstream>
#include <Memory/Memory.hpp>
#include <utils/bitoperations.hpp>

Memory::Memory() {

    hasBootedUp = true; // TODO: change it to false and find a way to set it to true when it has booted up
    romBankNumber = 1;
    ramBankNumber = 0;
    ramEnabled = false;
    romMode = true;

    // Boot rom
    uint8_t bootstrap_rom[256] = {
            0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
            0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
            0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
            0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
            0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
            0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
            0x1E, 0x02, 0x0E, 0x0c, 0xf0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
            0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
            0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
            0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
            0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
            0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
            0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
            0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
            0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
            0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
    };

    // Load the bootstrap rom to ram
    for (int i = 0; i < 256; i++){
        bios[i] = bootstrap_rom[i];
    }

    // Power Up Sequence http://bgb.bircd.org/pandocs.htm#powerupsequence
    io_registers[0x00] = 0xCF;
    io_registers[0x05] = 0x00;
    io_registers[0x06] = 0x00;
    io_registers[0x07] = 0x00;
    io_registers[0x10] = 0x80;
    io_registers[0x11] = 0xBF;
    io_registers[0x12] = 0xF3;
    io_registers[0x14] = 0xBF;
    io_registers[0x16] = 0x3F;
    io_registers[0x17] = 0x00;
    io_registers[0x19] = 0xBF;
    io_registers[0x1A] = 0x7F;
    io_registers[0x1B] = 0xFF;
    io_registers[0x1C] = 0x9F;
    io_registers[0x1E] = 0xBF;
    io_registers[0x20] = 0xFF;
    io_registers[0x21] = 0x00;
    io_registers[0x22] = 0x00;
    io_registers[0x23] = 0xBF;
    io_registers[0x24] = 0x77;
    io_registers[0x25] = 0xF3;
    io_registers[0x26] = 0xF1;
    io_registers[0x40] = 0x91;
    io_registers[0x42] = 0x00;
    io_registers[0x43] = 0x00;
    io_registers[0x45] = 0x00;
    io_registers[0x47] = 0xFC;
    io_registers[0x48] = 0xFF;
    io_registers[0x49] = 0xFF;
    io_registers[0x4A] = 0x00;
    io_registers[0x4B] = 0x00;
    interrupt_register = 0x00;

}

Memory::~Memory() {
    delete[] cart_rom;
    delete[] external_ram;
}

void Memory::loadGame(const char *romPath) {

    int fsize;
    std::ifstream rom(romPath, std::ios::binary | std::ios::ate);
    if (rom.fail()){
        throw std::runtime_error("Error: file");
    }

    fsize = rom.tellg();
    cart_rom = new uint8_t[fsize];
    rom.seekg(0, std::ios::beg);
    rom.read(reinterpret_cast<char *>(cart_rom), fsize);

    rom.close();

    uint8_t cart_type = cart_rom[0x0147];
    if (cart_type == 0) {
        mbc_type = MBC_TYPE::RomOnly;
    }
    else if (cart_type <= 3) {
        mbc_type = MBC_TYPE::MBC1;
    }
    else if (cart_type == 5 || cart_type == 6) {
        mbc_type = MBC_TYPE::MBC2;
    }
    else if (cart_type >= 0x0F && cart_type <= 0x13) {
        mbc_type = MBC_TYPE::MBC3;
    }
    else if (cart_type >= 0x15 && cart_type <= 0x17) {
        mbc_type = MBC_TYPE::MBC4;
    }
    else if (cart_type >= 0x19 && cart_type <= 0x1E) {
        mbc_type = MBC_TYPE::MBC5;
    }
    else {
        mbc_type = MBC_TYPE::Unknown;
    }

    uint8_t ram_size = cart_rom[0x0149];
    if (ram_size == 0) {
        external_ram = new uint8_t[512];    // for mbc2
    }
    else if (ram_size == 1) {
        external_ram = new uint8_t[2 * 1024];
    }
    else if (ram_size == 2) {
        external_ram = new uint8_t[8 * 1024];
    }
    else if (ram_size == 3) {
        external_ram = new uint8_t[32 * 1024];
    }
    else if (ram_size == 4) {
        external_ram = new uint8_t[128 * 1024];
    }
    else if (ram_size == 5) {
        external_ram = new uint8_t[64 * 1024];
    }
}

uint8_t Memory::readByte(uint16_t address) {
    if (address < 256 && !hasBootedUp) {
        return bios[address];
    }
    if (address < 0x4000) {
        return cart_rom[address];
    }
    if (address < 0x8000) {
        address = (romBankNumber * 0x4000) + (address - 0x4000);    // 0x4000 = 16KBytes the size of each bank
        return cart_rom[address];
    }
    if (address < 0xA000) {
        return vram[address - 0x8000];
    }
    if (address < 0xC000) {
        if (mbc_type == MBC_TYPE::MBC3) {
            // TODO: RTC register
        }
        else {
            if (ramEnabled) {
                address = (ramBankNumber * 0x2000) + (address - 0xA000);    // 0x2000 = 8KBytes the size of each bank
                return external_ram[address];
            }
        }
        throw std::runtime_error("Tried to read from external ram while ram its disabled");
    }
    if (address < 0xE000) {
        return wram[address - 0xC000];
    }
    if (address < 0xFE00) {
        // TODO echo ram
    }
    if (address < 0xFEA0) {
        return oam[address - 0xFE00];
    }
    if (address < 0xFF80 && address > 0xFEFF) {     // 0xFEA0-0xFEFF is unusable
        return io_registers[address - 0xFF00];
    }
    if (address < 0xFFFF) {
        return hram[address - 0xFF80];
    }
    if (address == 0xFFFF) {
        return interrupt_register;
    }
    throw std::runtime_error("Tried to read from invalid memory address");
}

void Memory::writeByte(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        enableRam(address, value);
        if (mbc_type == MBC_TYPE::MBC3) {
            // TODO: enable RTC Registers
        }
    }
    else if (address < 0x4000) {
        if (mbc_type == MBC_TYPE::MBC1) {       // change the lower 5 bits of rom bank number
            uint8_t lower5bits = value & 0x1F;
            romBankNumber &= 0xE0;      // set the lower 5 bits to 0 and keep the upper 3 the same
            romBankNumber |= lower5bits;
            if (romBankNumber == 0) {
                romBankNumber++;
            }
        }
        else if (mbc_type == MBC_TYPE::MBC2) {
            uint8_t upperAddress = address >> 8;
            if (checkBit(upperAddress, 0)) {   // the least significant bit of the upper byte of the address must be 1 to select rom bank
                romBankNumber = value & 0x0F;
                if (romBankNumber == 0) {
                    romBankNumber++;
                }
            }
        }
        else if (mbc_type == MBC_TYPE::MBC3) {
            romBankNumber = value & 0x7F;   // 7 bit
            if (romBankNumber == 0) {
                romBankNumber++;
            }
        }
        else if (mbc_type == MBC_TYPE::MBC5) {
            if (address < 0x3000) {
                romBankNumber = value;
            }
            else {
                value &= 0x01;
                romBankNumber = (value << 8) | romBankNumber;
            }
        }
    }
    else if (address < 0x6000) {
        if (mbc_type == MBC_TYPE::MBC1) {
            if (romMode) {      // change bit 5 and 6 of rom bank number
                romBankNumber &= 0x1F;  // set the first 3 bits to 0 and keep the lower 5 the same
                value &= 0xE0;      // set the lower 5 bits to 0
                romBankNumber |= value;
                if (romBankNumber == 0) {
                    romBankNumber++;
                }
            }
            else {
                ramBankNumber = value & 0x03;   // max of 4 ram banks need 2 bits to encode
            }
        }
        else if (mbc_type == MBC_TYPE::MBC3) {
            if (value >= 0x08 && value <= 0x0C) {
                // TODO: RTC register
            }
            else {
                ramBankNumber = value & 0x03;
            }
        }
        else if (mbc_type == MBC_TYPE::MBC5) {
            ramBankNumber = value & 0x0F;
        }
    }
    else if (address < 0x8000) {
        if (mbc_type == MBC_TYPE::MBC1) {
            if (!checkBit(value, 0)) {
                romMode = true;
                ramBankNumber = 0;
            }
            else {
                romMode = false;
            }
        }
        if (mbc_type == MBC_TYPE::MBC3) {
            // TODO: clock
        }
    }
    else if (address < 0xA000) {
        vram[address - 0x8000] = value;
    }
    else if (address < 0xC000) {
        if (mbc_type == MBC_TYPE::MBC3) {
            // TODO: RTC register
        }
        else {
            if (ramEnabled) {
                address = (ramBankNumber * 0x2000) + (address - 0xA000);
                external_ram[address] = value;
            }
        }
    }
    else if (address < 0xE000) {
        wram[address - 0xC000] = value;
    }
    else if (address < 0xFE00) {
        // TODO: echo ram
    }
    else if (address < 0xFEA0) {
        oam[address - 0xFE00] = value;
    }
    else if (address >= 0xFF00 && address <= 0xFF7F) {
        if (address == 0xFF04) {    // memory[0xFF04] = Divider Register. Writing any value sets it to 0
            io_registers[4] = 0;
        }
        else if (address == 0xFF44) {   // memory[0xFF44] = LCDC Y-Coordinate. Writing any value sets it to 0
            io_registers[0x44] = 0;
        }
        else if (address == 0xFF46) {   // memory[0xFF46] = DMA transfer
            DMATransfer(value);
        }
        else {
            io_registers[address - 0xFF00] = value;
        }
    }
    else if (address < 0xFFFF) {
        hram[address - 0xFF80] = value;
    }
    else if (address == 0xFFFF) {
        interrupt_register = value;
    }
    else {
        throw std::runtime_error("Tried to write to invalid memory address");
    }
}

void Memory::enableRam(uint16_t address, uint8_t value) {
    if (mbc_type == MBC_TYPE::MBC2) {
        uint8_t upperAddress = address >> 8;
        if (checkBit(upperAddress, 0)) {       // enable/disable only if the least significant bit of the upper byte of the address is 0
            return;
        }
    }
    ramEnabled = (value & 0x0F) == 0x0A;    // true if the lower nibble is 0xA
}

void Memory::DMATransfer(uint8_t value) {
    // The written value specifies the transfer source address divided by 0x100
    uint16_t address = value * 0x100;

    // Destination address space: FE00-FE9F
    for (int i = 0; i < 0xA0; i++) {
        writeByte(0xFE00 + i, readByte(address + i));
    }
}

// Timer counter
uint8_t Memory::getTIMA() {
    return io_registers[5];
}

void Memory::setTIMA(uint8_t value) {
    io_registers[5] = value;
}

// Timer modulo. When the TIMA overflows, this data will be loaded
uint8_t Memory::getTMA() {
    return io_registers[6];
}

void Memory::setTMA(uint8_t value) {
    io_registers[6] = value;
}

// Timer Control. Bit 2 = Timer Stop 0 Stop - 1 Start,
// bits 1-0 = Input Clock Select 00 4096Hz, 01 262144Hz, 10 65536Hz, 11 16384Hz
uint8_t Memory::getTAC() {
    return io_registers[7];
}

void Memory::setTAC(uint8_t value) {
    io_registers[7] = value;
}

// Divider Register. This register is incremented 16384 times a second
uint8_t Memory::getDIV() {
    return io_registers[4];
}

void Memory::setDIV(uint8_t value) {
    io_registers[4] = value;
}

// Interrupt Request Flag(IF) and Interrupt Enable(IE) Register
// Bit 4: Transition from High to Low of Pin number P10-P13
// Bit 3: Serial I/O transfer complete
// Bit 2: Timer Overflow
// Bit 1: LCDC
// Bit 0: V-Blank
uint8_t Memory::getIF() {
    return io_registers[0xF];
}

void Memory::setIF(uint8_t value) {
    io_registers[0xF] = value;
}

uint8_t Memory::getIE() {
    return interrupt_register;
}

void Memory::setIE(uint8_t value) {
    interrupt_register = value;
}

// LCD Control
// Bit 7 - LCD Control Operation
// Bit 6 - Window Tile Map Display Select
// Bit 5 - Window Display
// Bit 4 - BG & Window Tile Data Select
// Bit 3 - BG Tile Map Display Select
// Bit 2 - OBJ (Sprite) Size
// Bit 1 - OBJ (Sprite) Display
// Bit 0 - BG & Window Display
uint8_t Memory::getLCDC() {
    return io_registers[0x40];
}

void Memory::setLCDC(uint8_t value) {
    io_registers[0x40] = value;
}

// LCDC Status
// Bits 6-3 - Interrupt Selection By LCDC Status
// Bit 6 - LYC=LY Coincidence (Selectable)
// Bit 5 - Mode 10
// Bit 4 - Mode 01
// Bit 3 - Mode 00
// Bit 2 - Coincidence Flag
// Bit 1-0 - Mode Flag
// 00: During H-Blank
// 01: During V-Blank
// 10: During Searching OAM-RAM
// 11: During Transferring Data to LCD Driver
uint8_t Memory::getSTAT() {
    return io_registers[0x41];
}

void Memory::setSTAT(uint8_t value) {
    io_registers[0x41] = value;
}

// LCDC Y-Coordinate
// The LY indicates the vertical line to which
// the present data is transferred to the LCD
// Driver. The LY can take on any value
// between 0 through 153. The values between
// 144 and 153 indicate the V-Blank period
uint8_t Memory::getLY() {
    return io_registers[0x44];
}

void Memory::setLY(uint8_t value) {
    io_registers[0x44] = value;
}

// LY Compare
// The LYC compares itself with the LY. If the
// values are the same it causes the STAT to
// set the coincident flag
uint8_t Memory::getLYC() {
    return io_registers[0x45];
}

void Memory::setLYC(uint8_t value) {
    io_registers[0x45] = value;
}

// Scroll Y. The Y Position of the background
uint8_t Memory::getSCY() {
    return io_registers[0x42];
}

void Memory::setSCY(uint8_t value) {
    io_registers[0x42] = value;
}

// Scroll X. The X Position of the background
uint8_t Memory::getSCX() {
    return io_registers[0x43];
}

void Memory::setSCX(uint8_t value) {
    io_registers[0x43] = value;
}

// Window Y Position. The Y Position of the viewing area
uint8_t Memory::getWY() {
    return io_registers[0x4A];
}

void Memory::setWY(uint8_t value) {
    io_registers[0x4A] = value;
}

// Window X Position. The X Positions -7 of the viewing area
// WX is offset from absolute screen
// coordinates by 7. Setting the window to
// WX=7, WY=0 will put the upper left corner
// of the window at absolute screen
// coordinates 0,0
uint8_t Memory::getWX() {
    return io_registers[0x4B] - 7;
}

void Memory::setWX(uint8_t value) {
    io_registers[0x4B] = value;
}

// Bit 7 - Not used
// Bit 6 - Not used
// Bit 5 - P15 Select Button Keys (0=Select)
// Bit 4 - P14 Select Direction Keys (0=Select)
// Bit 3 - P13 Input Down or Start (0=Pressed) (Read Only)
// Bit 2 - P12 Input Up or Select (0=Pressed) (Read Only)
// Bit 1 - P11 Input Left or Button B (0=Pressed) (Read Only)
// Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
uint8_t Memory::getJoypadState() {
    return io_registers[0];
}

void Memory::setJoypadState(uint8_t keys_pressed) {
    // if button keys are selected or if direction keys are selected
    if (!checkBit(io_registers[0], 5)) {
        uint8_t topNibble = io_registers[0] & 0xF0;
        // button keys are in the upper nibble
        io_registers[0] = keys_pressed >> 4;
        io_registers[0] |= topNibble;
    }
    else if (!checkBit(io_registers[0], 4)) {
        uint8_t topNibble = io_registers[0] & 0xF0;
        // direction keys are in the lower nibble
        io_registers[0] = keys_pressed & 0x0F;
        io_registers[0] |= topNibble;
    }

}

void Memory::requestInterrupt(uint8_t interruptID) {
    uint8_t interrupt_requests = getIF();
    interrupt_requests = setBit(interrupt_requests, interruptID);
    setIF(interrupt_requests);
}
