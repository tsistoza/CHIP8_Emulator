#include "platform.hpp"

namespace Layer {
    Platform::Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(title, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    }
}