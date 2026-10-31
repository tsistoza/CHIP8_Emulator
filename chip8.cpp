#include <chip8.hpp>
#include <iostream>
#include <fstream>
#include <ios>
#include <random>
#include <chrono>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

const unsigned int FONTSET_SIZE = 80;
const uint8_t fontset[FONTSET_SIZE] = { // Each row represents a character --> 1 = 0010(2) 0000(0) looks like a 1 in binary
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0                                             0110(6) 0000
    0x20, 0x60, 0x20, 0x20, 0x70, // 1                                             0010    0000
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2                                             0010    0000
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3                                             0111(7) 0000
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const int _VIDEO_WIDTH = 64;
const int _VIDEO_HEIGHT = 32;
namespace Emulator {
    Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) { // Initialize RandGen
        pc = START_ADDRESS;

        // Load Fonts into memory
        for (unsigned int i=0; i<FONTSET_SIZE; i++)
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        VIDEO_WIDTH = _VIDEO_WIDTH;
        VIDEO_HEIGHT = _VIDEO_HEIGHT;
        randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

        table0.fill(OP_NULL);
        table8.fill(OP_NULL);
        tableE.fill(OP_NULL);
        tableF.fill(OP_NULL);

        table[0x0] = &Table0;
        table[0x1] = &OP_1nnn;
        table[0x2] = &OP_2nnn;
        table[0x3] = &OP_3xkk;
        table[0x4] = &OP_4xkk;
        table[0x5] = &OP_5xy0;
        table[0x6] = &OP_6xkk;
        table[0x7] = &OP_7xkk;
        table[0x8] = &Table8;
        table[0x9] = &OP_9xy0;
        table[0xA] = &OP_Annn;
        table[0xB] = &OP_Bnnn;
        table[0xC] = &OP_Cxkk;
        table[0xD] = &OP_Dxyn;
        table[0xE] = &TableE;
        table[0xF] = &TableF;

        table0[0x0] = &OP_00E0;
        table0[0xE] = &OP_00EE;
        
        table8[0x0] = &OP_8xy0;
        table8[0x1] = &OP_8xy1;
        table8[0x2] = &OP_8xy2;
        table8[0x3] = &OP_8xy3;
        table8[0x4] = &OP_8xy4;
        table8[0x5] = &OP_8xy5;
        table8[0x6] = &OP_8xy6;
        table8[0x7] = &OP_8xy7;
        table8[0xE] = &OP_8xyE;

        tableE[0x1] = &OP_ExA1;
        tableE[0xE] = &OP_Ex9E;

        tableF[0x07] = &OP_Fx07;
        tableF[0x0A] = &OP_Fx0A;
        tableF[0x15] = &OP_Fx15;
        tableF[0x18] = &OP_Fx18;
        tableF[0x1E] = &OP_Fx1E;
        tableF[0x29] = &OP_Fx29;
        tableF[0x33] = &OP_Fx33;
        tableF[0x55] = &OP_Fx55;
        tableF[0x65] = &OP_Fx65;
    }

    Chip8::~Chip8() {
        return;
    }

    void Chip8::loadROM(const char* filename) {
        // Open File as a stream of binary and move file pointer to end
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (file.is_open()) {
            // Get size of file and allocate buffer
            std::streampos size = file.tellg();
            char* buffer = new char[size];

            // Go to beginning of file, and fill buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();
            
            // load instructions to memory
            for (long i=0; i<size; i++) memory[START_ADDRESS + i] = buffer[i];

            delete[] buffer;
        }
        return;
    }

    // Instruction Definitions

    void Chip8::OP_00E0() {
        memset(video, 0, sizeof(video));
    }

    void Chip8::OP_00EE() {
        sp--;
        pc = stack[sp];
    }

    void Chip8::OP_1nnn() {
        uint16_t address = opcode & 0x0FFFu;
        pc = address;
    }

    void Chip8::OP_2nnn() {
        stack[sp++] = pc;
        uint16_t address = opcode & 0x0FFFu;
        pc = address;
    }

    void Chip8::OP_3xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FFu);
        if (registers[Vx] == kk) pc += 2;
    }

    void Chip8::OP_4xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FFu);
        if (registers[Vx] != kk) pc += 2;
    }

    void Chip8::OP_5xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
        if (registers[Vx] == registers[Vy]) pc += 2;
    }

    void Chip8::OP_6xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FFu);
        registers[Vx] = kk;
    }

    void Chip8::OP_7xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FFu);
        registers[Vx] += kk;
    }

    void Chip8::OP_8xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
        registers[Vx] = registers[Vy];
    }

    void Chip8::OP_8xy1() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[Vx] |= registers[Vy];
    }

    void Chip8::OP_8xy2() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[Vx] &= registers[Vy];
    }

    void Chip8::OP_8xy3() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[Vx] ^= registers[Vy];
    }

    void Chip8::OP_8xy4() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
        uint16_t sum = registers[Vx] + registers[Vy];
        registers[0xF] = (sum > 255) ? 1 : 0;
        registers[Vx] = sum & 0xFFu;
    }

    void Chip8::OP_8xy5() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[0xF] = (registers[Vx] > registers[Vy]) ? 1 : 0;
        registers[Vx] -= registers[Vy];
    }

    void Chip8::OP_8xy6() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        registers[0xF] = (registers[Vx] & 0x1u);
        registers[Vx] >>= 1;
    }

    void Chip8::OP_8xy7() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
        registers[0xF] = (registers[Vy] > registers[Vx]) ? 1 : 0;
        registers[Vx] = registers[Vy] - registers[Vx];
    }

    void Chip8::OP_8xyE() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        registers[0xF] = ((registers[Vx] & 0x80u) >> 7u);
        registers[Vx] <<= 1;
    }

    void Chip8::OP_9xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
        if (registers[Vx] != registers[Vy]) pc += 2;
    }

    void Chip8::OP_Annn() {
        uint16_t address = (opcode & 0x0FFFu);
        index = address;
    }

    void Chip8::OP_Bnnn() {
        uint16_t address = (opcode & 0x0FFFu);
        pc = address + registers[0x0];
    }

    void Chip8::OP_Cxkk() {
        uint8_t random = randByte(randGen);
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FFu);
        registers[Vx] = random & kk;
    }

    void Chip8::OP_Dxyn() {
        uint8_t height = static_cast<uint8_t>((opcode & 0x000Fu));
        uint8_t Vx = static_cast<uint8_t>(((opcode & 0x0F00u) >> 8u));
        uint8_t Vy = static_cast<uint8_t>(((opcode & 0x00F0u) >> 4u));

        uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
        uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

        registers[0xF] = 0;

        for (unsigned int i=0; i<height; i++) {
            uint8_t spriteByte = memory[index + i];

            for (unsigned int j=0; j<8; j++) {
                uint8_t spritePixel = spriteByte & (0x80u >> j);
                uint32_t* screenPixel = &video[(yPos + i) * VIDEO_WIDTH + (xPos + j)];

                if (spritePixel) {
                    if (*screenPixel == 0xFFFFFFFF) registers[0xF] = 1;
                    *screenPixel ^= 0xFFFFFFFF;
                }
            }
        }
    }

    void Chip8::OP_Ex9E() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t key = registers[Vx];
        
        if (keypad[key]) pc += 2;
    }

    void Chip8::OP_ExA1() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t key = registers[Vx];
        if (!keypad[key]) pc += 2;
    }

    void Chip8::OP_Fx07() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        registers[Vx] = delayTimer;
    }

    void Chip8::OP_Fx0A() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

        if (keypad[0]) registers[Vx] = 0;
        else if (keypad[1]) registers[Vx] = 1;
        else if (keypad[2]) registers[Vx] = 2;
        else if (keypad[3]) registers[Vx] = 3;
        else if (keypad[4]) registers[Vx] = 4;
        else if (keypad[5]) registers[Vx] = 5;
        else if (keypad[6]) registers[Vx] = 1;
        else if (keypad[7]) registers[Vx] = 2;
        else if (keypad[8]) registers[Vx] = 3;
        else if (keypad[9]) registers[Vx] = 4;
        else if (keypad[10]) registers[Vx] = 5;
        else if (keypad[11]) registers[Vx] = 1;
        else if (keypad[12]) registers[Vx] = 2;
        else if (keypad[13]) registers[Vx] = 3;
        else if (keypad[14]) registers[Vx] = 4;
        else if (keypad[15]) registers[Vx] = 5;
        else pc -= 2;
    }

    void Chip8::OP_Fx15() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        delayTimer = registers[Vx];
    }

    void Chip8::OP_Fx18() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        soundTimer = registers[Vx];
    }

    void Chip8::OP_Fx1E() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        index += registers[Vx];
    }

    void Chip8::OP_Fx29() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t digit = registers[Vx];
        index = FONTSET_START_ADDRESS + (5 * digit);
    }

    void Chip8::OP_Fx33() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        uint8_t value = registers[Vx];
        
        memory[index + 2] = value % 10;
        value /= 10;
        memory[index + 1] = value % 10;
        value /= 10;
        memory[index] = value % 10;
    }

    void Chip8::OP_Fx55() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        for (uint8_t i=0; i<=Vx; i++) memory[index + i] = registers[i];
    }

    void Chip8::OP_Fx65() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
        for (uint8_t i=0; i<=Vx; i++) registers[i] = memory[index + i];
    }

    void Chip8::OP_NULL() {}

    void Chip8::Table0() {
        (this->*table0[opcode & 0x000Fu])();
        
    }

    void Chip8::Table8() {
        (this->*table8[opcode & 0x000Fu])();
    }

    void Chip8::TableE() {
        (this->*tableE[opcode& 0x000Fu])();
    }

    void Chip8::TableF() {
        (this->*tableF[opcode & 0x00FFu])();
    }

    void Chip8::cycle() {
        opcode = (memory[pc] << 8u) | memory[pc + 1];
        pc += 2;
        (this->*table[(opcode & 0xF000u) >> 12u])();
        if (delayTimer > 0) delayTimer--;
        if (soundTimer > 0) soundTimer--;
    }
}