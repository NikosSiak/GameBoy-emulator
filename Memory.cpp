//
// Created by nikos on 29-Jul-19.
//

#include <algorithm>
#include <fstream>
#include "Memory.hpp"

Memory::Memory() {

    std::ifstream file("resources/bootstrap.bin", std::ios::binary | std::ios::ate);
    std::fill_n(ram, 0xFFFF, 0);

    if (file.good()){
        int fsize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(ram), fsize);
        file.close();
    }

}