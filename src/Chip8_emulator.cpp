#include "Chip8_emulator.h"

Chip8_emulator::Chip8_emulator(bool test) {
    this->test = test;
    this->m_screen = new Screen();
    this->m_chip8 = new Chip8(m_screen);
    //ctor
}

Chip8_emulator::~Chip8_emulator() {
    delete m_screen;
    delete m_chip8;
}

void Chip8_emulator::load_cartridge(const std::string& path) {
    if (test)
        m_screen->test_display();

    m_chip8->load_cartridge(path);

    if (test)
        m_chip8->verify_implementation();
}

void Chip8_emulator::run_emulator() {
    m_chip8->run_cartridge();
}
