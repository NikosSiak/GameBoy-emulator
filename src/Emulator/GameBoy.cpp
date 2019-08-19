//
// Created by nikos on 30-Jul-19.
//

#include <Emulator/GameBoy.hpp>

GameBoy::GameBoy() : cpu(memory), timers(memory), gpu(memory) {}

void GameBoy::loadGame(const char *romPath) {
    this->memory.loadGame(romPath);
}

void GameBoy::emulate() {
    int cycles = cpu.emulateInstruction();
    while(cycles != 0) {
        timers.updateTimers(cycles);
        gpu.updateGraphics(cycles);
        cpu.checkInterrupts();
        cycles = cpu.emulateInstruction();
    }
}
