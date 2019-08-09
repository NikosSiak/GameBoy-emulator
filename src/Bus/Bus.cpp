//
// Created by nikos on 09-Aug-19.
//

#include "Bus.hpp"

uint8_t Bus::readByteFromMemory(uint16_t address) {
    return m_memory.readByte(address);
}

void Bus::writeByteToMemory(uint16_t address, uint8_t value) {
    m_memory.writeByte(address, value);
}
