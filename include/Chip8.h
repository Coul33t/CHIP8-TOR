#ifndef CHIP8_H
#define CHIP8_H

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <array>

#include <stdlib.h> // rand

#include <memory>

#include "Screen.h"

struct instruction {
    // INCLUSIVE boundary for opcodes corresponding to the function
    std::pair<uint16_t, uint16_t> range;
    // Succinct name of the function associated with the opcode
    std::string name;
    // Pointer to the function associated with the opcode
    std::function<void(void)> func;

    // Ctor
    template<typename Func>
    instruction(std::pair<uint16_t, uint16_t> range, std::string name, Func func) : range(range), name(name), func(func) {}
};

class Chip8 {
    public:
        Chip8(Screen*);
        virtual ~Chip8();

        void set_opcodes(void);

        bool load_cartridge(std::string path);
        void output_cartridge_contents(void);
        bool verify_implementation(bool verbose = false);
        bool run_instructions(bool step_by_step = false, bool verbose = false);
        void run_cartridge(void);

        void draw_graphics(void);
        void check_keys(void);

        bool check_borrow(uint8_t reg1, uint8_t reg2);
        bool check_carry(uint8_t reg1, uint8_t reg2);

        void MULTI00CC(void);       // 0x00CC
        void DISPLAYCLEAR(void);    // 0x00E0
        void RETURN(void);          // 0x00EE

        void JUMP(void);            // 0x1NNN
        void CALL(void);            // 0x2NNN
        void SKIPVXNNEQ(void);      // 0x3XNN
        void SKIPVXNNDIFF(void);    // 0x4XNN
        void SKIPVXYEQ(void);       // 0x5XY0
        void SETVXNN(void);         // 0x6XNN
        void ADDNNVX(void);         // 0x7XNN

        void MULTI8XYC(void);       // 0x8XYC
        void SETVXVY(void);         // 0x8XY0
        void SETVXVXORVY(void);     // 0x8XY1
        void SETVXVXANDVY(void);    // 0x8XY2
        void SETVXVXXORVY(void);    // 0x8XY3
        void ADDVYVXCARRY(void);    // 0x8XY4
        void SUBVYVXCARRY(void);    // 0x8XY5
        void STORELSBVXVF(void);    // 0x8XY6
        void SETVXVYMINUSVX(void);  // 0x8XY7
        void STOREMSBVXVF(void);    // 0x8XYE

        void SETINN(void);          // 0xANNN
        void RAND(void);            // 0xCXNN
        void DRAW(void);            // 0xDXYN

        void MULTIEXCC(void);       // 0xEXCC
        void KEYPRESS(void);        // 0xEX9E
        void KEYNPRESS(void);       // 0xEXA1

        void MULTIFXCC(void);       // 0xFXCC
        void SETVXDTIMER(void);     // 0xFX07
        void KEYPRESSWAIT(void);    // 0xFX0A
        void SETDTIMERVX(void);     // 0xFX15
        void SETSTIMERVX(void);     // 0xFX18
        void ADDVXI(void);          // 0xFX1E
        void SETILOCSPRITE(void);   // 0xFX29
        void STOREBCVXI(void);      // 0xFX33
        void STOREV0VXI(void);      // 0xFX55
        void FILLV0VXI(void);       // 0xFX65

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

        // Stack
        std::vector<uint16_t> stack;
        // Stack pointer
        uint16_t sp;

        std::vector<instruction> instr;

        // Timers
        uint8_t delay_timer;
        uint8_t sound_timer;

        std::vector<uint8_t> key;

        uint16_t opcode;

        // Screen
        std::array<uint8_t, 64 * 32> gfx;
        bool draw_flag;

        // Reference to the screen
        Screen* screen;

};

#endif // CHIP8_H
