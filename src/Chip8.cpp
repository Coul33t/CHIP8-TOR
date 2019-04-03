#include "Chip8.h"

Chip8::Chip8() {
    this->memory = std::vector<uint8_t>(4096, 0x00);
    this->V = std::vector<uint8_t>(16, 0x00);

    this->pc = 0x0200;
    this->I = 0x0000;
    this->sp = 0x0000;

    this->stack = std::vector<uint16_t>(16, 0x0000);

    this->delay_timer = 0x00;
    this->sound_timer = 0x00;

    this->key = std::vector<uint8_t>(16, 0x00);

    this->draw_flag = false;

    this->set_opcodes();

    // Init random generator (fixed seed for reproducible bugs)
    srand(42);
}

Chip8::~Chip8() {
    //dtor
}

bool Chip8::load_cartridge(std::string path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);

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

bool Chip8::verify_implementation(bool verbose) {
    for (size_t i = 0x200; i < this->memory.size(); i += 2) {
        this->opcode = (uint8_t)this->memory[i] << 8 | (uint8_t)this->memory[i + 1];

        bool found = false;

        for (unsigned int i = 0; i < this->instr.size(); i++) {
            if ((this->instr[i].range.first <= this->opcode) && (this->instr[i].range.second >= this->opcode)) {
                if (verbose)
                    std::cout << std::hex << " implemented (" << this->instr[i].name << ")" << std::endl;
                found = true;
                break;
            }
        }

        if(!found) {
            if (verbose)
                std::cout << " not implemented." << std::endl;
            return false;
        }

    }

    return true;
}

bool Chip8::run_instructions(bool step_by_step, bool verbose) {
    for (size_t i = 0x200; i < this->memory.size(); i += 2) {
        this->opcode = (uint8_t)this->memory[i] << 8 | (uint8_t)this->memory[i + 1];

        if (verbose)
            std::cout << "Current opcode: " << std::hex << this->opcode;

        if (step_by_step)
            std::cin.get();

        bool found = false;

        for (unsigned int i = 0; i < this->instr.size(); i++) {
            if ((this->instr[i].range.first <= this->opcode) && (this->instr[i].range.second >= this->opcode)) {
                if (verbose)
                    std::cout << std::hex << " implemented (" << this->instr[i].name << ")" << std::endl;

                this->instr[i].func();
                found = true;
                break;
            }
        }

        if(!found) {
            if (verbose)
                std::cout << " not implemented." << std::endl;
            return false;
        }

    }

    return true;
}

void Chip8::run_cartridge(void) {
    bool found = false;

    while(true) {
        found = false;
        this->opcode = (uint8_t)this->memory[this->pc] << 8 | (uint8_t)this->memory[this->pc + 1];

        for (unsigned int i = 0; i < this->instr.size(); i++) {
            if ((this->instr[i].range.first <= this->opcode) && (this->instr[i].range.second >= this->opcode)) {
                this->instr[i].func();
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "ERROR: Could not fetch opcode (" << std::hex << this->opcode << ")." << std::endl;
            return;
        }
    }
}

bool Chip8::check_borrow(uint8_t reg1, uint8_t reg2) {
    return (reg1 < reg2);
}

bool Chip8::check_carry(uint8_t reg1, uint8_t reg2) {
    if (reg1 > 256 - reg2)
        return true;
    if (reg2 > 256 - reg1)
        return true;
    return false;
}

void Chip8::MULTI00CC(void) {
    uint16_t masking = this->opcode & 0x00FF;

    switch(masking) {
    case 0x0000:
        break;
    case 0x00E0:
        DISPLAYCLEAR();
        break;
    case 0x00EE:
        RETURN();
        break;
    default:
        std::cout << "WARNING: Specific function of 0x00CC not implemented. (" << std::hex << masking << ")" << std::endl;
        break;
    }
}

// OPCODE 0x00E0
// Clears screen display
void Chip8::DISPLAYCLEAR(void) {
    std::fill(this->gfx.begin(), this->gfx.end(), 0x0000);
    this->draw_flag = true;
    this->pc += 2;
}

// OPCODE 0x00EE
// Returns from a subroutine
void Chip8::RETURN(void) {
    this->sp--;
    this->pc = this->stack[this->sp];
    this->pc += 2;
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
    this->sp++;
    this->pc = this->opcode & 0x0FFF;
}

// OPCODE 0x3XNN
// Skips the next instruction if VX == NN
void Chip8::SKIPVXNNEQ(void) {
    if(this->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x4XNN
// Skips the next instruction if VX != NN
void Chip8::SKIPVXNNDIFF(void) {
    if(this->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x5XY0
// Skips the next instruction if VX == VY
void Chip8::SKIPVXYEQ(void) {
    if(this->V[(opcode & 0x0F00) >> 8] == this->V[(opcode & 0x00F0) >> 4])
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0x6XNN
// Sets VX to NN
void Chip8::SETVXNN(void) {
    this->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
    this->pc += 2;
}

// OPCODE 0x7XNN
// Adds NN to VX (no changes to carry flag)
void Chip8::ADDNNVX(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = (this->opcode & 0x00FF);
    this->pc += 2;
}

// OPCODE 0x8XCC
void Chip8::MULTI8XYC(void) {
    uint16_t masking = this->opcode & 0xF00F;

    switch(masking) {
    case 0x8000:
        SETVXVY();
        break;
    case 0x8001:
        SETVXVXORVY();
        break;
    case 0x8002:
        SETVXVXANDVY();
        break;
    case 0x8003:
        SETVXVXXORVY();
        break;
    case 0x8004:
        ADDVYVXCARRY();
        break;
    case 0x8005:
        SUBVYVXCARRY();
        break;
    case 0x8006:
        STORELSBVXVF();
        break;
    case 0x8007:
        SETVXVYMINUSVX();
        break;
    case 0x800E:
        STOREMSBVXVF();
        break;
    default:
        std::cout << "WARNING: Specific function of 0x8XYC not implemented." << std::endl;
        break;
    }
}

// OPCODE 0x8XY0
// Sets VX to the value of VY
void Chip8::SETVXVY(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4];
    this->pc += 2;
}

// OPCODE 0x8XY1
// Sets VX to VX | VY
void Chip8::SETVXVXORVY(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = (this->V[(this->opcode & 0x0F00) >> 8] | this->V[(this->opcode & 0x00F0) >> 4]);
    this->pc += 2;
}

// OPCODE 0x8XY2
// Sets VX to VX & VY
void Chip8::SETVXVXANDVY(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = (this->V[(this->opcode & 0x0F00) >> 8] & this->V[(this->opcode & 0x00F0) >> 4]);
    this->pc += 2;
}

// OPCODE 0x8XY3
// Sets VX to VX ^ VY (XOR)
void Chip8::SETVXVXXORVY(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = (this->V[(this->opcode & 0x0F00) >> 8] ^ this->V[(this->opcode & 0x00F0) >> 4]);
    this->pc += 2;
}

// OPCODE 0x8XY4
// Adds VY to VX (carry flag set)
void Chip8::ADDVYVXCARRY(void) {
    if (check_carry(this->V[(this->opcode & 0x0F00) >> 8], this->V[(this->opcode & 0x00F0) >> 4]))
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;
    this->V[(this->opcode & 0x0F00) >> 8] += (this->V[(this->opcode & 0x0F00) >> 8]);
    this->pc += 2;
}

// OPCODE 0x8XY5
// Subtracts VY to VX (carry flag set)
void Chip8::SUBVYVXCARRY(void) {
    if (check_borrow(this->V[(this->opcode & 0x0F00) >> 8], this->V[(this->opcode & 0x00F0) >> 4]))
        this->V[0xF] = 0;
    else
        this->V[0xF] = 1;
    this->V[(this->opcode & 0x0F00) >> 8] -= (this->V[(this->opcode & 0x0F00) >> 8]);
    this->pc += 2;
}

// OPCODE 0x8XY6
// Stores the LSB of VX into VF and shift VX >> 1
void Chip8::STORELSBVXVF(void) {
    this->V[0xF] = (this->V[(this->opcode & 0x0F00) >> 8] & 0x000E);
    this->V[(this->opcode & 0x0F00) >> 8] >>= 1;
    this->pc += 2;
}

// OPCODE 0x8XY7
// Sets VX to VY - VX (carry flag set)
void Chip8::SETVXVYMINUSVX(void) {
    if (check_borrow(this->V[(this->opcode & 0x0F00) >> 8], this->V[(this->opcode & 0x00F0) >> 8]))
        this->V[0xF] = 0;
    else
        this->V[0xF] = 1;
    this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4] - this->V[(this->opcode & 0x0F00) >> 8];
    this->pc += 2;
}

// OPCODE 0x8XYE
// Stores the MSB of VX into VF and shift VX >> 1
void Chip8::STOREMSBVXVF(void) {
    this->V[0xF] = (this->V[(this->opcode & 0x0F00) >> 8] & 0x1000);
    this->V[(this->opcode & 0x0F00) >> 8] <<= 1;
    this->pc += 2;
}

// OPCODE 0xANNN
// Sets I to NNN
void Chip8::SETINN(void) {
    this->I = (this->opcode & 0x0FFF);
    this->pc += 2;
}

// OPCODE 0XCXNN
//  Sets VX to a rand number
void Chip8::RAND(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = (rand() % 0xFF + 1) & (this->opcode & 0x00FF);
    this->pc += 2;
}

// OPCODE 0XDXYN
// Draws a sprite at VX, VY location, width = 8px, height = Npx
void Chip8::DRAW(void) {
    uint8_t x = this->V[(this->opcode & 0x0F00) >> 8];
    uint8_t y = this->V[(this->opcode & 0x00F0) >> 4];
    uint8_t h = this->V[this->opcode & 0x00F];
}

// OPCODE 0xEXCC
void Chip8::MULTIEXCC(void) {
    uint16_t masking = this->opcode & 0xF0FF;

    switch(masking) {
    case 0xE09E:
        KEYPRESS();
        break;
    case 0xE0A1:
        KEYNPRESS();
        break;
    default:
        std::cout << "WARNING: Specific function of 0xEXCC not implemented." << std::endl;
        break;
    }
}

// OPCODE 0xEX9E
// Skips the next instruction if key[VX] is pressed
void Chip8::KEYPRESS(void) {
    if (this->key[this->V[(this->opcode & 0x0F00) >> 8]] != 0)
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0xEXA1
// Skips the next instruction if key[VX] isn't pressed
void Chip8::KEYNPRESS(void) {
    if (this->key[this->V[(this->opcode & 0x0F00) >> 8]] == 0)
        this->pc += 4;
    else
        this->pc += 2;
}

// OPCODE 0xFXCC
void Chip8::MULTIFXCC(void) {
    uint16_t masking = this->opcode & 0xF0FF;

    switch(masking) {
    case 0xF007:
        SETVXDTIMER();
        break;

    case 0xF00A:
        KEYPRESSWAIT();
        break;

    case 0xF015:
        SETDTIMERVX();
        break;

    case 0xF018:
        SETSTIMERVX();
        break;

    case 0xF01E:
        ADDVXI();
        break;

    case 0xF029:
        SETILOCSPRITE();
        break;

    case 0xF033:
        STOREBCVXI();
        break;

    case 0xF055:
        STOREV0VXI();
        break;

    case 0xF065:
        FILLV0VXI();
        break;

    default:
        std::cout << "WARNING: Specific function of 0xFXCC not implemented." << std::endl;
        break;
    }
}

// OPCODE 0xFX07
// Sets VX to delay timer
void Chip8::SETVXDTIMER(void) {
    this->V[(this->opcode & 0x0F00) >> 8] = this->delay_timer;
    this->pc += 2;
}

// OPCODE 0xFX0A
// Waits for a key press, then store in VX (blocking)
void Chip8::KEYPRESSWAIT(void) {
    bool key_pressed = false;
    while(!key_pressed) {
        for (unsigned int i= 0; i < this->key.size(); i++) {
            if (this->key[i] != 0) {
                this->V[(this->opcode & 0x0F00) >> 8] = i;
                key_pressed = true;
                break;
            }
        }
    }

    this->pc += 2;
}

// OPCODE 0xFX15
// Sets delay timer to VX
void Chip8::SETDTIMERVX(void) {
    this->delay_timer = this->V[(this->opcode & 0x0F00) >> 8];
    this->pc += 2;
}

// OPCODE 0xFX18
// Sets sound timer to VX
void Chip8::SETSTIMERVX(void) {
    this->sound_timer = this->V[(this->opcode & 0x0F00) >> 8];
    this->pc += 2;
}

// OPCODE 0xFX1E
// Adds VX to I
void Chip8::ADDVXI(void) {
    if (check_carry(this->I, this->V[(this->opcode & 0x0F00) >> 8]))
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;

    this->I += this->V[(this->opcode & 0x0F00) >> 8];
    this->pc += 2;
}

// OPCODE 0xFX29
// Sets I to the location of the sprite for the character in VX
void Chip8::SETILOCSPRITE(void) {
    this->I = this->V[(this->opcode & 0x0F00) >> 8] * 0x5;
    this->pc += 2;
}
// OPCODE 0xFX33
// Stores the binary-coded decimal representation of VX, with the most significant of three digits
// at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
void Chip8::STOREBCVXI(void) {
    this->memory[this->I]     = this->V[(this->opcode & 0x0F00) >> 8] / 100;
    this->memory[this->I + 1] = (this->V[(this->opcode & 0x0F00) >> 8] / 10) % 10;
    this->memory[this->I + 2] = this->V[(this->opcode & 0x0F00) >> 8] % 10;
    this->pc += 2;
}

// OPCODE 0xFX55
// Stores V0 to VX (included) in memory starting at I
void Chip8::STOREV0VXI(void) {
    unsigned int limit = ((this->opcode & 0x0F00) >> 8);

    for (unsigned int i = 0; i < limit; i++)
        this->memory[this->I + i] = this->V[i];

    this->I += limit;
    this->pc += 2;
}

// OPCODE 0xFX65
// Fills V0 to VX (included) with memory starting at I
void Chip8::FILLV0VXI(void) {
    unsigned int limit = ((this->opcode & 0x0F00) >> 8);

    for (unsigned int i = 0; i < limit; i++)
        this->V[i] = this->memory[this->I + i];

    this->I += limit;
    this->pc += 2;
}

void Chip8::set_opcodes(void) {
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x0000, 0x0FFF), std::string("00CC"), [this] {this->MULTI00CC();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x1000, 0x1FFF), std::string("JUMP TO NNN"), [this] {this->JUMP();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x2000, 0x2FFF), std::string("CALL NNN"), [this] {this->CALL();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x3000, 0x3FFF), std::string("SKIP IF X == NN"), [this] {this->SKIPVXNNEQ();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x4000, 0x4FFF), std::string("SKIP IF X != NN"), [this] {this->SKIPVXNNDIFF();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x5000, 0x5FFF), std::string("SKIP IF X == Y"), [this] {this->SKIPVXYEQ();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x6000, 0x6FFF), std::string("SET X TO NN"), [this] {this->SETVXNN();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x7000, 0x7FFF), std::string("ADD NN TO VX"), [this] {this->ADDNNVX();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0x8000, 0x8FFF), std::string("8XYC"), [this] {this->MULTI8XYC();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0xA000, 0xAFFF), std::string("SET I TO NNN"), [this] {this->SETINN();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0xC000, 0xCFFF), std::string("RAND"), [this] {this->RAND();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0xD000, 0xDFFF), std::string("DRAW AT XY"), [this] {this->DRAW();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0xE000, 0xEFFF), std::string("EXCC"), [this] {this->MULTIEXCC();}));
    this->instr.push_back(instruction(std::pair<uint16_t, uint16_t>(0xF000, 0xFFFF), std::string("FXCC"), [this] {this->MULTIFXCC();}));
}
