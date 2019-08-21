//
// Created by nikos on 30-Jul-19.
//

#ifndef GAMEBOY_EMULATOR_GAMEBOY_HPP
#define GAMEBOY_EMULATOR_GAMEBOY_HPP


#include <CPU/CPU.hpp>
#include <GPU/GPU.hpp>
#include <Memory/Memory.hpp>
#include <Timers/Timers.hpp>
#include <Joypad/Joypad.hpp>

class GameBoy {

    CPU cpu;
    GPU gpu;
    Memory memory;
    Timers timers;
    Joypad joypad;

public:
    GameBoy();
    void loadGame(const char *romPath);
    void emulate();
    GPU &getGPU();
    Joypad &getJoypad();
};


#endif //GAMEBOY_EMULATOR_GAMEBOY_HPP
