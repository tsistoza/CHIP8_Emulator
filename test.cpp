#include "test.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

#define assert(expression, message)

namespace Suite {
    Tests::Tests(Emulator::Chip8 _cpu) {
        cpu = _cpu;
    }

    void Tests::ClearRegisters() {
        std::fill_n(cpu.registers, 16, 0);
        std::fill_n(cpu.memory, 4096, 0);
        cpu.index = 0x0;
        cpu.pc = 0x200;
        std::fill_n(cpu.stack, 16, 0);
        cpu.sp = 0x0;
        cpu.delayTimer = 0x0;
        cpu.soundTimer = 0x0;
        std::fill_n(cpu.keypad, 16, 0);
        std::fill_n(cpu.video, 64*32, 0);
        cpu.opcode = 0x0;
    }

    void Tests::chip8InitTest() {
        assert(pc == 0x200 && "pc is not initialized properly!");

        const int FONTSET_START_ADDRESS = 0x50;
        assert(cpu.memory[FONTSET_START_ADDRESS] == 0xF0, "fonts not initialized");
        assert(cpu.memory[FONTSET_START_ADDRESS+79] == 0x80, "fonts not initialized");
    }

    void Tests::OP_00E0_Test() {
        cpu.OP_00E0();
        assert(cpu.video[0] == 0, "OP_00E0 is not working");
    }

    void Tests::OP_00EE_Test() {
        cpu.stack[cpu.sp] = 1;
        cpu.sp++;
        cpu.OP_00EE();
        assert(cpu.pc == 1, "OP_00EE is not working");
    }

    void Tests::OP_1nnn_Test() {
        cpu.opcode = 0x1FFFu;
        cpu.OP_1nnn();
        assert(cpu.pc == 0x0FFF, "OP_1nnn is not working");
    }

    void Tests::OP_2nnn_Test() {
        cpu.opcode = 0x2FFFu;
        cpu.OP_2nnn();
        assert(cpu.pc == 0x0FFFu, "OP_2nnn is not working");
    }

    void Tests::OP_3xkk_Test() {
        cpu.opcode = 0x31FF;
        cpu.pc = 0x200;
        cpu.registers[1] = 0xFFu;
        cpu.OP_3xkk();
        assert(cpu.pc == 0x200 + 2u, "OP_3xkk is not working");
    }

    void Tests::OP_4xkk_Test() {
        cpu.opcode = 0x31FF;
        cpu.pc = 0x200;
        cpu.registers[1] = 0x0u;
        cpu.OP_3xkk();
        assert(cpu.pc == 0x200 + 2u, "OP_4xkk is not working");
    }

    void Tests::OP_5xy0_Test() {
        cpu.opcode = 0x512F;
        cpu.pc = 0x200;
        cpu.registers[1] = 0xFu;
        cpu.registers[2] = 0xFu;
        cpu.OP_5xy0();
        assert(cpu.pc == 0x200 + 2u, "OP_5xy0 is not working");
    }

    void Tests::OP_6xkk_Test() {
        cpu.opcode = 0x61FFu;
        cpu.OP_6xkk();
        assert(cpu.registers[1] == 0xFFu, "OP_6xkk is not working")
    }

    void Tests::OP_7xkk_Test() {
        cpu.opcode = 0x7101u;
        cpu.registers[1] = 0x0;
        cpu.OP_7xkk();
        assert(cpu.registers[1] == 1u, "OP_7xkk is not working");
    }

    void Tests::OP_8xy0_Test() {
        cpu.opcode = 0x8120u;
        cpu.registers[2] = 0xFu;
        cpu.OP_8xy0();
        assert(cpu.registers[1] == 0xFu, "OP_8xy0 is not working");
    }

    void Tests::OP_8xy1_Test() {
        cpu.opcode = 0x8121u;
        cpu.registers[1] = 0x00u;
        cpu.registers[2] = 0xFFu;
        cpu.OP_8xy1();
        assert(cpu.registers[1] == 0xFFu, "OP_8xy1 is not working");
    }

    void Tests::OP_8xy2_Test() {
        cpu.opcode = 0x8121u;
        cpu.registers[1] = 0xFFu;
        cpu.registers[2] = 0xF0u;
        cpu.OP_8xy2();
        assert(cpu.registers[1] == 0xF0u, "OP_8xy2 is not working");
    }

    void Tests::OP_8xy3_Test() {
        cpu.opcode = 0x8123u;
        cpu.registers[1] = 0xF0u;
        cpu.registers[2] = 0x0Fu;
        cpu.OP_8xy3();
        assert(cpu.registers[1] == 0xFFu, "OP_8xy3 is not working");
    }

    void Tests::OP_8xy4_Test() {
        cpu.opcode = 0x8124u;
        cpu.registers[1] = 0xFFu;
        cpu.registers[2] = 0x01u;
        cpu.OP_8xy4();
        assert(cpu.registers[1] == 0x0, "OP_8xy4 is not working");
    }

    void Tests::OP_8xy5_Test() {
        cpu.opcode = 0x8125u;
        cpu.registers[1] = 0x0Fu;
        cpu.registers[2] = 0xFFu;
        cpu.OP_8xy5();
        assert(cpu.registers[1] == (0x0Fu-0xFFu), "OP_8xy5 is not working");
    }

    void Tests::OP_8xy6_Test() {
        cpu.opcode = 0x8126u;
        cpu.registers[1] = 0x02u;
        cpu.OP_8xy6();
        assert((cpu.registers[1] == 0x1u && cpu.registers[0xF] == 1), "OP_8xy6 is not working");
    }

    void Tests::OP_8xy7_Test() {
        cpu.opcode = 0x8127u;
        cpu.registers[1] = 0x01u;
        cpu.registers[2] = 0x02u;
        cpu.OP_8xy7();
        assert(cpu.registers[0xF] == 0x1u, "OP_8xy7 is not working");
    }

    void Tests::OP_8xyE_Test() {
        cpu.opcode = 0x812Eu;
        cpu.registers[1] = 0x01u;
        cpu.OP_8xyE();
        assert(cpu.registers[0xF] == 0x2u, "OP_8xyE is not working");
    }

    void Tests::OP_9xy0_Test() {
        cpu.opcode = 0x9120u;
        cpu.pc = 0x200u;
        cpu.registers[1] = 0x01u;
        cpu.registers[2] = 0x02u;
        cpu.OP_9xy0();
        assert(cpu.pc == 0x202u, "OP_9xy0 is not working");
    }

    void Tests::OP_Annn_Test() {
        cpu.opcode = 0xAFFF;
        cpu.OP_Annn();
        assert(cpu.index == 0xFFFu, "OP_Annn is not working");
    }

    void Tests::OP_Bnnn_Test() {
        cpu.opcode = 0xB001u;
        cpu.pc = 0x200u;
        cpu.registers[0] = 0x0u;
        uint16_t sum = cpu.registers[0] + 0x001u;
        cpu.OP_Bnnn();
        assert(cpu.pc == sum, "OP_Bnnn is not working");
    }

    void Tests::OP_Cxkk_Test() {
        cpu.opcode = 0xC100u;
        cpu.OP_Cxkk();
        assert(cpu.registers[1] == 0x0u, "OP_Cxkk is not working");
    }

    void Tests::OP_Dxyn_Test() {
        
    }
}