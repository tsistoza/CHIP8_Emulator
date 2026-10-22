#include "chip8.hpp"

namespace Suite {
    class Tests {
        public:
            Tests(Emulator::Chip8 _cpu);
        
            void chip8InitTest();
            void OP_00E0_Test();
            void OP_00E0_Test();
            void OP_00EE_Test();
            void OP_1nnn_Test();
            void OP_2nnn_Test();
            void OP_3xkk_Test();
            void OP_4xkk_Test();
            void OP_5xy0_Test();
            void OP_6xkk_Test();
            void OP_7xkk_Test();
            void OP_8xy0_Test();
            void OP_8xy1_Test();
            void OP_8xy2_Test();
            void OP_8xy3_Test();
            void OP_8xy4_Test();
            void OP_8xy5_Test();
            void OP_8xy6_Test();
            void OP_8xy7_Test();
            void OP_8xyE_Test();
            void OP_9xy0_Test();
            void OP_Annn_Test();
            void OP_Bnnn_Test();
            void OP_Cxkk_Test();
            void OP_Dxyn_Test();
            void OP_Ex9E_Test();
            void OP_ExA1_Test();
            void OP_Fx07_Test();
            void OP_Fx0A_Test();
            void OP_Fx18_Test();
            void OP_Fx1E_Test();
            void OP_Fx29_Test();
            void OP_Fx33_Test();
            void OP_Fx55_Test();
            void OP_Fx65_Test();
        private:
            Emulator::Chip8 cpu;
    };
}
