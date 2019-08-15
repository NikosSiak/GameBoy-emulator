//
// Created by nikos on 30-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_GAMEBOY_HPP
#define GAMEBOY_EMULATOR_GAMEBOY_HPP


#include <CPU/CPU.hpp>
#include <Memory/Memory.hpp>
#include <Timers/Timers.hpp>

class GameBoy {

    CPU cpu;
    Memory memory;
    Timers timers;

public:
    GameBoy();
    void loadGame(const char *romPath);
    void emulate();
};


#endif //GAMEBOY_EMULATOR_GAMEBOY_HPP
