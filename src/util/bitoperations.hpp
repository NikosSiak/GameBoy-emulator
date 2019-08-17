//
// Created by nikos on 17-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_BITOPERATIONS_HPP
#define GAMEBOY_EMULATOR_BITOPERATIONS_HPP

#include <cstdint>

uint8_t setBit(uint8_t byte, int bit);

uint8_t resetBit(uint8_t byte, uint8_t bit);

bool checkBit(uint8_t byte, uint8_t bit);

#endif //GAMEBOY_EMULATOR_BITOPERATIONS_HPP
