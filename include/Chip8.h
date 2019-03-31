#ifndef CHIP8_H
#define CHIP8_H

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>

struct instruction {
    uint16_t op_code;
    uint16_t mask;
    std::string name;
    void* func;
};

class Chip8 {
    public:
        Chip8();
        virtual ~Chip8();

        void set_opcodes(void);

        bool load_cartridge(std::string path);
        void output_cartridge_contents(void);
        bool verifiy_instructions(void);

        // 0     -> 0x1FF: interpreter (unused since the emulator is running outside of the memory)
        // 0x200 -> 0xE9F: cartridge data
        // 0xEA0 -> 0xEFF: call stack, internal use, and other variables
        // 0xF00 -> 0xFFF: display refresh
        std::vector<uint8_t> memory;
        std::vector<uint8_t> registers;

        // Index register
        uint16_t I;
        // Program counter
        uint16_t pc;

        std::vector<uint8_t> stack;
        // Stack pointer
        uint8_t sp;

        std::vector<instruction> instr;

        // Timers
        uint8_t delay_timer;
        uint8_t sound_timer;

        // Screen
        bool gfx[64 * 32];

};

#endif // CHIP8_H
