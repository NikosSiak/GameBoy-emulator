//
// Created by nikos on 04-Oct-20.
//

#include <GPU/Colour.hpp>

Colour getColour(uint8_t value) {
    switch (value) {
        case 0: return Colour::White;
        case 1: return Colour::Light_Grey;
        case 2: return Colour::Dark_Grey;
        default: return Colour::Black;
    }
}
