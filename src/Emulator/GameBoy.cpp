//
// Created by nikos on 30-Jul-19.
//

#include <Emulator/GameBoy.hpp>

GameBoy::GameBoy() : cpu(memory), timers(memory), gpu(memory), joypad(memory) {}

void GameBoy::loadGame(const char *romPath) {
    this->memory.loadGame(romPath);
}

void GameBoy::emulate() {
    // the amount of clock cycles the gameboy can exectue every second is 4194304
    // which means that if each frame we update the emulator 60 times a second the each
    // frame will execute 69905(4194304/60) clock cycles
    const int MAXCYCLES = 69905;
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < MAXCYCLES) {
        int cycles = cpu.emulateInstruction();
        cyclesThisUpdate += cycles;
        timers.updateTimers(cycles);
        gpu.updateGraphics(cycles);
        cpu.checkInterrupts();
    }
}
