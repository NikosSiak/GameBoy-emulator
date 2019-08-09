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
    Bus bus;

public:
    GameBoy() : bus(ram), cpu(bus) {}
    void loadGame(const char *romPath);
    void emulateCpu();
};


#endif //GAMEBOY_EMULATOR_GAMEBOY_HPP