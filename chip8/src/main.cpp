#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>
#include "SDL2/SDL.h"
#include "chip8.h"

// Keypad keymap
uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

int main(int argc, char **argv) {
    setenv("LIBGL_ALWAYS_INDIRECT", "1", 1);
    setenv("SDL_VIDEO_X11_VISUALID", "", 1);
    
    if (argc !=2) {
        std::cout << "Usage: chip8 <ROM>" << std::endl;    
        return 1;        
    }

    int width = 512;
    int height = 256;

    SDL_Window* window = NULL;
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow("CHIP-8 Emulator",SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width,height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        exit(2);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, 
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    64,32);
    uint32_t pixels[2048];
    Chip8 chip8;
    if (!chip8.loadROM(argv[1])) {
        std::cerr << "Error loading ROM" << std::cout;
        return 2;
    }

    while (true) {
        chip8.cycle();
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                exit(0);

            if(e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);
                if (e.key.keysym.sym == SDLK_F1)
                    chip8.loadROM(argv[1]);
                //TODO get rid of for loop, use a hashtable
                for (int i = 0; i < 16; ++i) {
                    if (e.key.keysym.sym == keymap[i]) {
                        chip8.keypad[i] = 1;
                    }
                }
            }
            if (e.type == SDL_KEYUP) {
                for (int i = 0; i < 16; ++i) {
                    if(e.key.keysym.sym == keymap[i]) {
                        chip8.keypad[i] = 0;
                    }
                }
            }
            if(chip8.drawFlag) {
                chip8.drawFlag = false;
                for (int i = 0; i < 2048; ++i) {
                    uint8_t pixel = chip8.gfx[i];
                    pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
                }
                SDL_UpdateTexture(sdlTexture, NULL, pixels, 64*sizeof(uint32_t));
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
            
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1200));

        }
    
    
    
    }
    return 0;
}
