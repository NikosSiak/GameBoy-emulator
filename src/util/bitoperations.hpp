//
// Created by nikos on 17-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_BITOPERATIONS_HPP
#define GAMEBOY_EMULATOR_BITOPERATIONS_HPP

#include <cstdint>

inline uint8_t setBit(uint8_t byte, int bit) {
    return byte | (1 << bit);
}

inline uint8_t resetBit(uint8_t byte, uint8_t bit) {
    return byte & ~(1 << bit);
}

inline bool checkBit(uint8_t byte, uint8_t bit) {
    return ((byte >> bit) & 1) == 1;
}

#endif //GAMEBOY_EMULATOR_BITOPERATIONS_HPP
