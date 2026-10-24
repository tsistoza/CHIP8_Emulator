#include "test.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

#define assert_msg(expression, message) assert((expression) && message)

namespace Suite {
    Tests::Tests(Emulator::Chip8* _cpu) {
        cpu = _cpu;
    }

    void Tests::startInstructionTest() {
        chip8InitTest();
        OP_00E0_Test();
        OP_00EE_Test();
        OP_1nnn_Test();
        OP_2nnn_Test();
        OP_3xkk_Test();
        OP_4xkk_Test();
        OP_5xy0_Test();
        OP_6xkk_Test();
        OP_7xkk_Test();
        OP_8xy0_Test();
        OP_8xy1_Test();
        OP_8xy2_Test();
        OP_8xy3_Test();
        OP_8xy4_Test();
        OP_8xy5_Test();
        OP_8xy6_Test();
        OP_8xy7_Test();
        OP_8xyE_Test();
        OP_9xy0_Test();
        OP_Annn_Test();
        OP_Bnnn_Test();
        OP_Cxkk_Test();
        OP_Dxyn_Test();
        OP_Ex9E_Test();
        OP_ExA1_Test();
        OP_Fx07_Test();
        OP_Fx0A_Test();
        OP_Fx15_Test();
        OP_Fx18_Test();
        OP_Fx1E_Test();
        OP_Fx29_Test();
        OP_Fx33_Test();
        OP_Fx55_Test();
        OP_Fx65_Test();
    }

    void Tests::ClearRegisters() {
        std::fill_n(cpu->registers, 16, 0);
        std::fill_n(cpu->memory, 4096, 0);
        cpu->index = 0x0;
        cpu->pc = 0x200;
        std::fill_n(cpu->stack, 16, 0);
        cpu->sp = 0x0;
        cpu->delayTimer = 0x0;
        cpu->soundTimer = 0x0;
        std::fill_n(cpu->keypad, 16, 0);
        std::fill_n(cpu->video, 64*32, 0);
        cpu->opcode = 0x0;
    }

    void Tests::chip8InitTest() {
        assert_msg(cpu->pc == 0x200, "pc is not initialized properly!");

        const int FONTSET_START_ADDRESS = 0x50;
        assert_msg(cpu->memory[FONTSET_START_ADDRESS] == 0xF0, "fonts not initialized");
        assert_msg(cpu->memory[FONTSET_START_ADDRESS+79] == 0x80, "fonts not initialized");
    }

    void Tests::OP_00E0_Test() {
        cpu->OP_00E0();
        assert_msg(cpu->video[0] == 0, "OP_00E0 is not working");
    }

    void Tests::OP_00EE_Test() {
        cpu->stack[cpu->sp] = 1u;
        cpu->sp++;
        cpu->OP_00EE();
        assert_msg(cpu->pc == 1u, "OP_00EE is not working");
    }

    void Tests::OP_1nnn_Test() {
        cpu->opcode = 0x1FFFu;
        cpu->OP_1nnn();
        assert_msg(cpu->pc == 0x0FFFu, "OP_1nnn is not working");
    }

    void Tests::OP_2nnn_Test() {
        cpu->opcode = 0x2FFFu;
        cpu->OP_2nnn();
        assert_msg(cpu->pc == 0x0FFFu, "OP_2nnn is not working");
    }

    void Tests::OP_3xkk_Test() {
        cpu->opcode = 0x31FFu;
        cpu->pc = 0x200u;
        cpu->registers[1] = 0xFFu;
        cpu->OP_3xkk();
        assert_msg(cpu->pc == 0x200u + 2u, "OP_3xkk is not working");
    }

    void Tests::OP_4xkk_Test() {
        cpu->opcode = 0x31FFu;
        cpu->pc = 0x200u;
        cpu->registers[1] = 0x0u;
        cpu->OP_4xkk();
        assert_msg(cpu->pc == 0x200u + 2u, "OP_4xkk is not working");
    }

    void Tests::OP_5xy0_Test() {
        cpu->opcode = 0x512F;
        cpu->pc = 0x200;
        cpu->registers[1] = 0xFu;
        cpu->registers[2] = 0xFu;
        cpu->OP_5xy0();
        assert_msg(cpu->pc == 0x200u + 2u, "OP_5xy0 is not working");
    }

    void Tests::OP_6xkk_Test() {
        cpu->opcode = 0x61FFu;
        cpu->OP_6xkk();
        assert_msg(cpu->registers[1] == 0xFFu, "OP_6xkk is not working");
    }

    void Tests::OP_7xkk_Test() {
        cpu->opcode = 0x7101u;
        cpu->registers[1] = 0x0;
        cpu->OP_7xkk();
        assert_msg(cpu->registers[1] == 1u, "OP_7xkk is not working");
    }

    void Tests::OP_8xy0_Test() {
        cpu->opcode = 0x8120u;
        cpu->registers[2] = 0xFu;
        cpu->OP_8xy0();
        assert_msg(cpu->registers[1] == 0xFu, "OP_8xy0 is not working");
    }

    void Tests::OP_8xy1_Test() {
        cpu->opcode = 0x8121u;
        cpu->registers[1] = 0x00u;
        cpu->registers[2] = 0xFFu;
        cpu->OP_8xy1();
        assert_msg(cpu->registers[1] == 0xFFu, "OP_8xy1 is not working");
    }

    void Tests::OP_8xy2_Test() {
        cpu->opcode = 0x8121u;
        cpu->registers[1] = 0xFFu;
        cpu->registers[2] = 0xF0u;
        cpu->OP_8xy2();
        assert_msg(cpu->registers[1] == 0xF0u, "OP_8xy2 is not working");
    }

    void Tests::OP_8xy3_Test() {
        cpu->opcode = 0x8123u;
        cpu->registers[1] = 0xF0u;
        cpu->registers[2] = 0x0Fu;
        cpu->OP_8xy3();
        assert_msg(cpu->registers[1] == 0xFFu, "OP_8xy3 is not working");
    }

    void Tests::OP_8xy4_Test() {
        cpu->opcode = 0x8124u;
        cpu->registers[1] = 0xFFu;
        cpu->registers[2] = 0x01u;
        cpu->OP_8xy4();
        assert_msg(cpu->registers[1] == 0x0, "OP_8xy4 is not working");
    }

    void Tests::OP_8xy5_Test() {
        cpu->opcode = 0x8125u;
        cpu->registers[1] = 0x0Fu;
        cpu->registers[2] = 0xFFu;
        cpu->OP_8xy5();
        assert_msg(cpu->registers[1] == (uint8_t)(0x0Fu-0xFFu), "OP_8xy5 is not working");
    }

    void Tests::OP_8xy6_Test() {
        cpu->opcode = 0x8126u;
        cpu->registers[1] = 0x02u;
        cpu->OP_8xy6();
        assert_msg((cpu->registers[1] == 1u && cpu->registers[0xF] == 0u), "OP_8xy6 is not working");
    }

    void Tests::OP_8xy7_Test() {
        cpu->opcode = 0x8127u;
        cpu->registers[1] = 0x01u;
        cpu->registers[2] = 0x02u;
        cpu->OP_8xy7();
        assert_msg(cpu->registers[0xF] == 0x1u, "OP_8xy7 is not working");
    }

    void Tests::OP_8xyE_Test() {
        cpu->opcode = 0x812Eu;
        cpu->registers[1] = 0x80u;
        cpu->OP_8xyE();
        assert_msg(cpu->registers[0xF] == 1u, "OP_8xyE is not working");
    }

    void Tests::OP_9xy0_Test() {
        cpu->opcode = 0x9120u;
        cpu->pc = 0x200u;
        cpu->registers[1] = 0x01u;
        cpu->registers[2] = 0x02u;
        cpu->OP_9xy0();
        assert_msg(cpu->pc == 0x202u, "OP_9xy0 is not working");
    }

    void Tests::OP_Annn_Test() {
        cpu->opcode = 0xAFFF;
        cpu->OP_Annn();
        assert_msg(cpu->index == 0xFFFu, "OP_Annn is not working");
    }

    void Tests::OP_Bnnn_Test() {
        cpu->opcode = 0xB001u;
        cpu->pc = 0x200u;
        cpu->registers[0] = 0x0u;
        uint16_t sum = cpu->registers[0] + 0x001u;
        cpu->OP_Bnnn();
        assert_msg(cpu->pc == sum, "OP_Bnnn is not working");
    }

    void Tests::OP_Cxkk_Test() {
        cpu->opcode = 0xC100u;
        cpu->OP_Cxkk();
        assert_msg(cpu->registers[1] == 0x0u, "OP_Cxkk is not working");
    }

    void Tests::OP_Dxyn_Test() {
        cpu->opcode = 0xD121u;
        cpu->registers[1] = 1;
        cpu->registers[2] = 2;
        cpu->index = 0x0;
        cpu->memory[0] = 0xFFu;
        
        cpu->video[129] = 0xFFFFFFFFu;
        cpu->OP_Dxyn();
        assert_msg(cpu->registers[0xF] == 1, "OP_Dxyn is not working");
    }

    void Tests::OP_Ex9E_Test() {
        cpu->opcode = 0xE19Eu;
        cpu->pc = 0x200u;
        cpu->registers[1] = 1;
        cpu->keypad[1] = 1;
        cpu->OP_Ex9E();
        assert_msg(cpu->pc == 0x202u, "OP_Ex9E is not working");
    }

    void Tests::OP_ExA1_Test() {
        cpu->opcode = 0xE1A1u;
        cpu->pc = 0x200u;
        cpu->registers[1] = 1;
        cpu->keypad[1] = 0;
        cpu->OP_ExA1();
        assert_msg(cpu->pc == 0x202u, "OP_ExA1 is not working");
    }

    void Tests::OP_Fx07_Test() {
        cpu->opcode = 0xF107u;
        cpu->delayTimer = 1u;
        cpu->OP_Fx07();
        assert_msg(cpu->registers[1] == 1u, "OP_Fx07 is not working");
    }

    void Tests::OP_Fx0A_Test() {
        cpu->opcode = 0xF10Au;
        cpu->keypad[1] = 1u;
        cpu->OP_Fx0A();
        assert_msg(cpu->registers[1] == 1u, "OP_Fx0A is not working");
    }

    void Tests::OP_Fx15_Test() {
        cpu->opcode = 0xF115u;
        cpu->registers[1] = 1u;
        cpu->delayTimer = 0u;
        cpu->OP_Fx15();
        assert_msg(cpu->delayTimer == 1u, "OP_Fx15 is not working");
    }

    void Tests::OP_Fx18_Test() {
        cpu->opcode = 0xF118u;
        cpu->registers[1] = 1u;
        cpu->soundTimer = 0u;
        cpu->OP_Fx18();
        assert_msg(cpu->soundTimer == 1u, "OP_Fx18 is not working");
    }

    void Tests::OP_Fx1E_Test() {
        cpu->opcode = 0xF11Eu;
        cpu->registers[1] = 1u;
        cpu->index = 0u;
        cpu->OP_Fx1E();
        assert_msg(cpu->index == 1u, "OP_Fx1E is not working");
    }

    void Tests::OP_Fx29_Test() {
        cpu->opcode = 0xF129u;
        cpu->registers[1] = 1u;
        cpu->index = 0u;
        cpu->OP_Fx29();
        assert_msg(cpu->index == (0x50u + 5), "OP_Fx29 is not working");
    }

    void Tests::OP_Fx33_Test() {
        cpu->opcode = 0xF133u;
        cpu->registers[1] = 100u;
        cpu->index = 0u;
        cpu->OP_Fx33();
        assert_msg(cpu->memory[0] == 1u, "OP_Fx33 is not working");
    }

    void Tests::OP_Fx55_Test() {
        cpu->opcode = 0xF155u;
        cpu->index = 0u;
        for (uint8_t i=0; i<=1; i++) cpu->registers[1] = 10u;
        for (uint8_t i=0; i<=1; i++) cpu->memory[i] = 0u;
        cpu->OP_Fx55();
        assert_msg(cpu->memory[1] == 10u, "OP_Fx55 is not working");
    }

    void Tests::OP_Fx65_Test() {
        cpu->opcode = 0xF165u;
        cpu->index = 0u;
        for (uint8_t i=0; i<=1; i++) cpu->registers[i] = 0u;
        for (uint8_t i=0; i<=1; i++) cpu->memory[i] = (uint8_t)i;
        cpu->OP_Fx65();
        assert_msg((cpu->registers[0] == 0u && cpu->registers[1] == 1u), "OP_Fx65 is not working");
    }
}

int main() {
    Emulator::Chip8 chp8;
    Suite::Tests test(&chp8);
    test.startInstructionTest();
    std::cout << "ALL TESTS PASSED!!!\n " << std::endl;
    return 0;
}