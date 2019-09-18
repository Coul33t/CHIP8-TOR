#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include "Chip8.h"
#include "Screen.h"

class Chip8_emulator
{
    public:
        Chip8_emulator();
        virtual ~Chip8_emulator();
        void load_cartridge(const std::string& path);
        void test_screen(void);
        void run_emulator(void);
    protected:
    private:
        Chip8 m_chip8;
        Screen m_screen;
};

#endif // CHIP8_EMULATOR_H
