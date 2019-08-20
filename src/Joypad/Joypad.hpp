//
// Created by nikos on 20-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_JOYPAD_HPP
#define GAMEBOY_EMULATOR_JOYPAD_HPP

#include <cstdint>

class Memory;

enum class Key {
    Right = 0,
    Left,
    Up,
    Down,
    A,
    B,
    Start,
    Select
};

class Joypad {

    Memory &m_memory;
    uint8_t keys_pressed = 0xFF;    // all keys unpressed

public:
    Joypad(Memory &memory);
    void keyPress(Key key);
    void keyRelease(Key key);

};


#endif //GAMEBOY_EMULATOR_JOYPAD_HPP
