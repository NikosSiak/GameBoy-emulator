//
// Created by nikos on 30-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_GAMEBOY_HPP
#define GAMEBOY_EMULATOR_GAMEBOY_HPP


#include <CPU/CPU.hpp>
#include <Memory/Memory.hpp>

class GameBoy {

    CPU cpu;
    Memory memory;

public:
    GameBoy();
    void loadGame(const char *romPath);
    void emulateCpu();
};


#endif //GAMEBOY_EMULATOR_GAMEBOY_HPP
