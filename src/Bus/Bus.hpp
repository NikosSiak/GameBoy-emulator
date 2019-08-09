//
// Created by nikos on 09-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_BUS_HPP
#define GAMEBOY_EMULATOR_BUS_HPP


#include <Memory/Memory.hpp>

class Bus {

    Memory &m_memory;

public:
    Bus(Memory &memory) : m_memory(memory) {}
    uint8_t readByteFromMemory(uint16_t address);
    void writeByteToMemory(uint16_t address, uint8_t value);
};


#endif //GAMEBOY_EMULATOR_BUS_HPP
