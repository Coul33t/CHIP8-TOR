#ifndef CHIP8_H
#define CHIP8_H

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>

struct instruction {
    std::pair<uint16_t, uint16_t> range;
    std::string name;
    std::function<void(void)> func;

    template<typename Func>
    instruction(std::pair<uint16_t, uint16_t> range, std::string name, Func func) : range(range), name(name), func(func) {}
};

class Chip8 {
    public:
        Chip8();
        virtual ~Chip8();

        void set_opcodes(void);

        bool load_cartridge(std::string path);
        void output_cartridge_contents(void);
        bool verifiy_instructions(void);

        void JUMP(void);            // 0x1NNN
        void CALL(void);            // 0x2NNN
        void SKIPXNNEQ(void);       // 0x3XNN
        void SKIPXNNDIFF(void);     // 0x4XNN
        void SKIPXYEQ(void);        // 0x5XY0
        void SETXNN(void);          // 0x6XNN

        // 0     -> 0x1FF: interpreter (unused since the emulator is running outside of the memory)
        // 0x200 -> 0xE9F: cartridge data
        // 0xEA0 -> 0xEFF: call stack, internal use, and other variables
        // 0xF00 -> 0xFFF: display refresh
        std::vector<uint8_t> memory;
        // registers, V0 to VF (V Flag)
        std::vector<uint8_t> V;

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

        uint16_t opcode;

        // Screen
        bool gfx[64 * 32];

};

#endif // CHIP8_H
