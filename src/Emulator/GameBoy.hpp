//
// Created by nikos on 30-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_GAMEBOY_HPP
#define GAMEBOY_EMULATOR_GAMEBOY_HPP


#include <CPU/CPU.hpp>
#include <Memory/Memory.hpp>

class GameBoy {

    Memory ram;
    CPU cpu;

public:
    void loadGame(const char *romPath);

};


#endif //GAMEBOY_EMULATOR_GAMEBOY_HPP
