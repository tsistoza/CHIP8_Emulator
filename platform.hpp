#pragma once
#include <cstdint>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

namespace Layer {
    class Platform {
        public:
            Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
            ~Platform();

            void Update(const void* buffer, int pitch);
            bool ProcessInput(uint8_t* keys);

        private:
            SDL_Window* window{};
            SDL_Renderer* renderer{};
            SDL_Texture* texture{};
    };
}