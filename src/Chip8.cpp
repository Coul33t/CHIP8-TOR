#include "Chip8.h"

Chip8::Chip8() {
    this->memory = std::vector<uint8_t>(4096, 0x00);
    this->V = std::vector<uint8_t>(16,0x00);

    this->pc = 0x0200;
    this->I = 0x0000;

    this->stack = std::vector<uint8_t>(16,0x00);

    this->delay_timer = 0x00;
    this->sound_timer = 0x00;

    this->set_opcodes();
}

Chip8::~Chip8() {
    //dtor
}

bool Chip8::load_cartridge(std::string path) {
    std::ifstream input(path, std::ios::binary|std::ios::ate);

    if (!input.is_open()) {
        std::cout << "ERROR: can't open binary file." << std::endl;
        return false;
    }

    std::ifstream::pos_type pos = input.tellg();
    size_t file_size = static_cast<size_t>(pos);

    std::vector<char> file_contents(file_size);

    input.seekg(0, std::ios::beg);
    input.read(&file_contents[0], pos);
    input.close();

    for (unsigned int i = 0; i < file_contents.size(); i++)
        this->memory[0x200 + i] = file_contents[i];

    return true;
}

bool Chip8::verifiy_instructions(void) {
    for (size_t i = 0x200; i < this->memory.size(); i += 2) {
        this->opcode = (uint8_t)this->memory[i] << 8 | (uint8_t)this->memory[i+1];

        std::cout << "Current opcode: " << std::hex << this->opcode;

        bool found = false;

        for (unsigned int i = 0; i < this->instr.size(); i++) {
            if ((this->instr[i].range.first < this->opcode) && (this->instr[i].range.second > this->opcode)) {
                std::cout << std::hex << " implemented (" << this->instr[i].name << ")" << std::endl;
                this->instr[i].func();
                found = true;
                break;
            }
        }

        if(!found) {
           std::cout << " not implemented." << std::endl;
        return false;
        }

    }

    return true;
}

// OPCODE 0x1NNN
// Jumps to address NNN
void Chip8::JUMP(void) {
    this->pc = (this->opcode & 0x0FFF);
}

// OPCODE 0x2NNN
// Calls subroutine at NNN
void Chip8::CALL(void) {
    this->stack[this->sp] = this->pc;
    ++sp;
    this->pc = this->opcode & 0x0FFF;
}

// OPCODE 0x3XNN
// Skips the next instruction if VX == NN
void Chip8::SKIPXNNEQ(void) {
    if(this->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x4XNN
// Skips the next instruction if VX != NN
void Chip8::SKIPXNNDIFF(void) {
    if(this->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x5XY0
// Skips the next instruction if VX == VY
void Chip8::SKIPXYEQ(void) {
    if(this->V[(opcode & 0x0F00) >> 8] == this->V[(opcode & 0x00F0) >> 4])
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x6XNN
// Sets VX to NN
void Chip8::SETXNN(void) {
    this->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
}


void Chip8::set_opcodes(void) {
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x1000,0x1FFF), std::string("JUMP"), [this]{this->JUMP();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x2000,0x2FFF), std::string("CALL"), [this]{this->CALL();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x3000,0x3FFF), std::string("SKIP IF X == NN"), [this]{this->SKIPXNNEQ();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x4000,0x4FFF), std::string("SKIP IF X != NN"), [this]{this->SKIPXNNDIFF();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x5000,0x5FFF), std::string("SKIP IF X == Y"), [this]{this->SKIPXYEQ();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x6000,0x6FFF), std::string("SET X TO NN"), [this]{this->SETXNN();}));
}
