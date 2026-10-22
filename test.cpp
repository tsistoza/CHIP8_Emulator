#include "test.hpp"
#include <iostream>
#include <cassert>

#define assert(expression, message)

namespace Suite {
    Tests::Tests(Emulator::Chip8 _cpu) {
        cpu = _cpu;
    }

    void Tests::chip8InitTest() {
        assert(pc == 0x200 && "pc is not initialized properly!");

        const int FONTSET_START_ADDRESS = 0x50;
        assert(cpu.memory[FONTSET_START_ADDRESS] == 0xF0, "fonts not initialized");
        assert(cpu.memory[FONTSET_START_ADDRESS+79] == 0x80, "fonts not initialized");
    }
}