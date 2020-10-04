//
// Created by nikos on 15-Aug-19.
//

#include <Timers/Timers.hpp>
#include <Memory/Memory.hpp>
#include <utils/bitoperations.hpp>
#include <cstdint>

Timers::Timers(Memory &memory) : m_memory(memory) {}

void Timers::updateTimers(int cycles) {
    updateDIVRegister(cycles);

    uint8_t freq = m_memory.getTAC() & 0b11;
    if (freq != frequency_clock_select) {
        setClockFreq();
        frequency_clock_select = freq;
    }

    if (isClockEnabled()) {
        timerCounter -= cycles;

        if (timerCounter <= 0) {
            // reset timer counter
            setClockFreq();

            // when TIMA overflows TMA is written to it
            if (m_memory.getTIMA() == 255) {
                m_memory.setTIMA(m_memory.getTMA());
                // set timer overflow interrupt
                // timer overflow interrupt is in bit 2 (counting from 0)
                m_memory.requestInterrupt(2);
            }
            else {
                m_memory.setTIMA(m_memory.getTIMA() + 1);
            }
        }
    }
}

bool Timers::isClockEnabled() {
    int8_t tac = m_memory.getTAC();
    if (checkBit(tac, 2)) {
        return true;
    }
    return false;
}

void Timers::setClockFreq() {
    uint8_t freq = m_memory.getTAC() & 0b11;  // bit 1-0 Input Clock Select 00 4096Hz, 01 262144Hz, 10 65536Hz, 11 16384Hz
    // timerCounter = 4194304 (clock speed of gb) / frequency
    switch (freq) {
        case 0: timerCounter = 1024;  break;
        case 1: timerCounter = 16;    break;
        case 2: timerCounter = 64;    break;
        case 3: timerCounter = 256;   break;
    }
}

// DIV register is incremented 16384 times a second
void Timers::updateDIVRegister(int cycles) {
    DIVRegisterCounter += cycles;
    if (DIVRegisterCounter >= 255) {  // 4194304 (clock speed of gb) / 16384 = 256 so it updates every 256 cpu cycles (0 - 255)
        DIVRegisterCounter = 0;
        m_memory.setDIV(m_memory.getDIV() + 1);
    }
}
