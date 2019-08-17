//
// Created by nikos on 17-Aug-19.
//

#include <util/bitoperations.hpp>

uint8_t setBit(uint8_t byte, int bit) {
    return byte | (1 << bit);
}

uint8_t resetBit(uint8_t byte, uint8_t bit) {
    return byte & ~(1 << bit);
}

bool checkBit(uint8_t byte, uint8_t bit) {
    return ((byte >> bit) & 1) == 1;
}
