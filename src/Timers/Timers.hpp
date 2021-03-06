//
// Created by nikos on 15-Aug-19.
//

#ifndef GAMEBOY_EMULATOR_TIMERS_HPP
#define GAMEBOY_EMULATOR_TIMERS_HPP

class Memory;

class Timers {

    Memory &m_memory;

    int DIVRegisterCounter = 0;
    int frequency_clock_select = 0;
    int timerCounter = 1024;  // 4194304 (clock speed of gb) / 4096 (default frequency)

    bool isClockEnabled();
    void setClockFreq();
    void updateDIVRegister(int cycles);

public:
    Timers(Memory &memory);
    void updateTimers(int cycles);
};


#endif //GAMEBOY_EMULATOR_TIMERS_HPP
