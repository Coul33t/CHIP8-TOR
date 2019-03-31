#include <iostream>
#include "Chip8.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    Chip8 c8;
    c8.load_cartridge("roms\\INVADERS");
    c8.verifiy_instructions();
    return 0;
}
