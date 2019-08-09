//
// Created by nikos on 30-Jul-19.
//

#include <Emulator/GameBoy.hpp>

void GameBoy::loadGame(const char *romPath) {
    this->ram.loadGame(romPath);
}

void GameBoy::emulateCpu() {
    cpu.emulateInstruction();
}
