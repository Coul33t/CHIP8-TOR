#include <iostream>
#include "Chip8.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    Chip8 c8;
    c8.load_cartridge("roms\\PONG");
    c8.verify_implementation();
    c8.run_instructions();
    c8.run_cartridge();
    return 0;
}
