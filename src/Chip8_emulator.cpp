#include "Chip8_emulator.h"

Chip8_emulator::Chip8_emulator() {
    //ctor
}

Chip8_emulator::~Chip8_emulator() {
    //dtor
}

void Chip8_emulator::load_cartridge(const std::string& path) {
    m_chip8.load_cartridge(path);
    m_chip8.verify_implementation();
}

void Chip8_emulator::run_emulator() {
    m_chip8.run_cartridge();
}
