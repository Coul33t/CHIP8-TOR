#include "Chip8.h"

Chip8::Chip8() {
    this->memory = std::vector<uint8_t>(4096, 0x00);
    this->registers = std::vector<uint8_t>(16,0x00);

    this->I = 0x0000;
    this->pc = 0x0000;

    this->stack = std::vector<uint8_t>(16,0x00);

    this->delay_timer = 0x00;
    this->sound_timer = 0x00;
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
        uint16_t op_code = (uint8_t)this->memory[i] << 8 | (uint8_t)this->memory[i+1];
        auto it = std::find_if(this->instr.begin(),
                               this->instr.end(),
                               [&op_code](instruction instruct) {return instruct.op_code & op_code != 0;}); // Nope, use masks in instr
        if (it == this->instr.end()) {
            std::cout << "ERROR: opcode " << std::hex << (unsigned)op_code << " not implemented." << std::endl;
            return false;
        }
    }

    return true;
}

void Chip8::set_opcodes(void) {
}
