#include <iostream>
#include <Emulator/GameBoy.hpp>

int main(int argc, char *argv[]) {
    GameBoy emulator;
    emulator.loadGame(argv[1]);
    return 0;
}
