#include "chip8.hpp"
#include "platform.hpp"
#include <iostream>
#include <chrono>

const int _VIDEO_WIDTH = 64;
const int _VIDEO_HEIGHT = 32;

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    int videoScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    const char* romFilename = argv[3];
    
    Layer::Platform platform("CHIP-8 Emu", _VIDEO_WIDTH * videoScale, _VIDEO_HEIGHT * videoScale, _VIDEO_WIDTH, _VIDEO_HEIGHT);

    Emulator::Chip8 chip8;
    chip8.loadROM(romFilename);

    int videoPitch = sizeof(chip8.video[0]) * _VIDEO_WIDTH;
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit) {
        quit = platform.ProcessInput(chip8.keypad);
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            chip8.cycle();
            platform.Update(chip8.video, videoPitch);
        }
    }

    return 0;
}