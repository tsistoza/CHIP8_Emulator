#include "chip8.hpp"
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

namespace Emulator {
    Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) { // Initialize RandGen
        pc = START_ADDRESS;

        // Load Fonts into memory
        for (unsigned int i=0; i<FONTSET_SIZE; i++)
            memory[FONTSET_START_ADDRESS + i] = fontset[i];

        randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
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
        pc = stack[sp--];
    }

    void Chip8::OP_1nnn() {
        uint16_t address = opcode & 0x0FFF;
        pc = address;
    }

    void Chip8::OP_2nnn() {
        stack[sp++] = pc;
        uint16_t address = opcode & 0x0FFF;
        pc = address;
    }

    void Chip8::OP_3xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FF);
        if (Vx == kk) pc += 2;
    }

    void Chip8::OP_4xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FF);
        if (Vx != kk) pc += 2;
    }

    void Chip8::OP_5xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        if (Vx == Vy) pc += 2;
    }

    void Chip8::OP_6xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode & 0x00FF);
        registers[Vx] = kk;
    }

    void Chip8::OP_7xkk() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode * 0x00FF);
        registers[Vx] += kk;
    }

    void Chip8::OP_8xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
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
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        if (registers[Vx] + registers[Vy] > 255) registers[0xF] = 1; 
        registers[Vx] += registers[Vy];
    }

    void Chip8::OP_8xy5() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[0xF] = (registers[Vx] > registers[Vy]) ? 1 : 0;
        registers[Vx] -= registers[Vy];
    }

    void Chip8::OP_8xy6() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        registers[0xF] = (registers[Vx] & 0x0001 == 1) ? 1 : 0;
        registers[Vx] /= 2;
    }

    void Chip8::OP_8xy7() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        registers[0xF] = (registers[Vy] > registers[Vx]) ? 1 : 0;
        registers[Vx] = registers[Vy] - registers[Vx];
    }

    void Chip8::OP_8xyE() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        registers[0xF] = (((registers[Vx] & 0x8000) >> 7) == 1) ? 1 : 0;
        registers[Vx] *= 2;
    }

    void Chip8::OP_9xy0() {
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t Vy = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
        if (registers[Vx] != registers[Vy]) pc += 2;
    }

    void Chip8::OP_Annn() {
        uint16_t address = (opcode & 0x0FFF);
        index = address;
    }

    void Chip8::OP_Bnnn() {
        uint16_t address = (opcode & 0x0FFF);
        pc = address + registers[0x0];
    }

    void Chip8::OP_Cxkk() {
        uint8_t random = randByte(randGen);
        uint8_t Vx = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
        uint8_t kk = static_cast<uint8_t>(opcode * 0x00FF);
        registers[Vx] = random & kk;
    }

    void Chip8::OP_Dxyn() {
        uint8_t nibble = static_cast<uint8_t>((opcode & 0x000F));
        
    }
}