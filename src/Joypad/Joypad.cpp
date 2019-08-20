//
// Created by nikos on 20-Aug-19.
//

#include <Joypad/Joypad.hpp>
#include <Memory/Memory.hpp>
#include <util/bitoperations.hpp>

Joypad::Joypad(Memory &memory) : m_memory(memory) {}

void Joypad::keyPress(Key key) {
    // pressed keys have value 0 not 1!!!
    // if key was not pressed
    if (checkBit(keys_pressed, static_cast<uint8_t>(key))) {
        uint8_t joypadState = m_memory.getJoypadState();

        keys_pressed = resetBit(keys_pressed, static_cast<uint8_t>(key));

        bool directionKey = true;
        if (static_cast<uint8_t>(key) > 3) {
            directionKey = false;
        }

        bool reqInterrupt = false;
        // Bit 5 - P15 Select Button Keys (0=Select)
        // Bit 4 - P14 Select Direction Keys (0=Select)
        if (!directionKey && !checkBit(joypadState, 5)) {
            reqInterrupt = true;
        } else if (directionKey && !checkBit(joypadState, 4)) {
            reqInterrupt = true;
        }

        if (reqInterrupt) {
            m_memory.requestInterrupt(4);
        }

        m_memory.setJoypadState(keys_pressed);
    }
}

void Joypad::keyRelease(Key key) {
    keys_pressed = setBit(keys_pressed, static_cast<uint8_t>(key));
    m_memory.setJoypadState(keys_pressed);
}
