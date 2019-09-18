#include <iostream>
#include "Chip8_emulator.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello world!" << std::endl;

    Chip8_emulator emu;
    emu.test_screen();
    emu.load_cartridge("roms\\PONG");
    emu.run_emulator();

    /*c8.load_cartridge("roms\\PONG");
    c8.verify_implementation();
    c8.run_instructions();
    c8.run_cartridge();*/
    return 0;
}
