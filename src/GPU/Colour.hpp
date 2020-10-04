//
// Created by nikos on 04-Oct-20.
//

#ifndef GAMEBOY_EMULATOR_COLOUR_HPP
#define GAMEBOY_EMULATOR_COLOUR_HPP

#include <cstdint>

enum class Colour {
    White,
    Light_Grey,
    Dark_Grey,
    Black
};

Colour getColour(uint8_t value);

#endif //GAMEBOY_EMULATOR_COLOUR_HPP
