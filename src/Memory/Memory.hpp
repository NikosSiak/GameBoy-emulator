//
// Created by nikos on 29-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_MEMORY_HPP
#define GAMEBOY_EMULATOR_MEMORY_HPP

// General Memory Map
// 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
// 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
// 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
// A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
// C000-CFFF   4KB Work RAM Bank 0 (WRAM)
// D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
// E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
// FE00-FE9F   Sprite Attribute Table (OAM)
// FEA0-FEFF   Not Usable
// FF00-FF7F   I/O Ports
// FF80-FFFE   High RAM (HRAM)
// FFFF        Interrupt Enable Register

#include <cstdint>

enum class MBC_TYPE {
    RomOnly,
    MBC1,
    MBC2,
    MBC3,
    MBC4,
    MBC5,
    Unknown
};

class Memory {

    uint8_t bios[256];
    uint8_t *cart_rom;
    uint8_t wram[8 * 1024];
    uint8_t *external_ram;
    uint8_t vram[8 * 1024];
    uint8_t oam[160];
    uint8_t io_registers[128];
    uint8_t hram[127];
    bool interrupt_register;

    bool hasBootedUp;
    bool ramEnabled;
    bool romMode;

    uint16_t romBankNumber;     // 16 bit to support mbc5 which needs 9 bits
    uint8_t ramBankNumber;

    MBC_TYPE mbc_type;

    void enableRam(uint16_t address, uint8_t value);

public:
    Memory();
    ~Memory();
    void loadGame(const char *romPath);
    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    uint8_t getTIMA();
    void setTIMA(uint8_t value);
    uint8_t getTMA();
    void setTMA(uint8_t value);
    uint8_t getTAC();
    void setTAC(uint8_t value);
    uint8_t getDIV();
    void setDIV(uint8_t value);
};


#endif //GAMEBOY_EMULATOR_MEMORY_HPP
