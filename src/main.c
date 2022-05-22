#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "chip8.h"

const char keyboardMap[CHIP8_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int artgc, char** argv) {
    struct chip8 chip8;
    chip8Init(&chip8);
    chip8.registers.soundTimer = 100;

    chip8ScreenDrawSprite(&chip8.screen, 62, 30, &chip8.memory.memory[0x00], 5);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        CHIP8_WIDTH * CHIP8_WINDOW_SIZE_MULTIPLIER, 
        CHIP8_HEIGHT * CHIP8_WINDOW_SIZE_MULTIPLIER, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(1) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    goto out;
                break;
                
                case SDL_KEYDOWN: {
                    char key = event.key.keysym.sym;
                    int vkey = chip8KeyboardMap(keyboardMap, key);
                    if(vkey != -1) {
                        chip8KeyDown(&chip8.keyboard, vkey);
                    }
                }
                break;

                case SDL_KEYUP: {
                    char key = event.key.keysym.sym;
                    int vkey = chip8KeyboardMap(keyboardMap, key);
                    if(vkey != -1) {
                        chip8KeyUp(&chip8.keyboard, vkey);
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for(int x = 0; x < CHIP8_WIDTH; x++) {
            for(int y = 0; y < CHIP8_HEIGHT; y++) {
                if(chip8ScreenIsSet(&chip8.screen, x, y)) {
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_SIZE_MULTIPLIER;
                    r.y = y * CHIP8_WINDOW_SIZE_MULTIPLIER;
                    r.w = CHIP8_WINDOW_SIZE_MULTIPLIER;
                    r.h = CHIP8_WINDOW_SIZE_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
        if(chip8.registers.delayTimer > 0) {
            Sleep(100);
            chip8.registers.delayTimer -= 1;
        }

        if(chip8.registers.soundTimer > 0) {
            Beep(2000, 100 * chip8.registers.soundTimer);
            chip8.registers.soundTimer = 0;
        }
    }
out:
    SDL_DestroyWindow(window);
    return 0;
}