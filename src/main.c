#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "chip8.h"

// Modern keyboard keys that will be converted to CHIP8 keyboard keys
const char keyboardMap[CHIP8_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char** argv) {
    // Checking if the ROM file is included and if it can open, pretty self explanatory
    if(argc < 2) {
        printf("You must provide a file to load!");
        return -1;
    }

    const char *fileName = argv[1];
    printf("The filename to load is: %s\n", fileName);

    FILE *f = fopen(fileName, "rb");
    if(!f) {
        printf("Failed to open file!");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buffer[size];
    int result = fread(buffer, size, 1, f);
    if(result != 1) {
        printf("Failed to read from file!");
        return -1;
    }

    // Initializing the chip8, making it load the memory and setting up the virtual keyboard
    struct chip8 chip8;
    chip8Init(&chip8);
    chip8Load(&chip8, buffer, size);
    chip8KeyboardSetMap(&chip8.keyboard, keyboardMap);

    // Initializes the emulator window with values defined in config.h
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        CHIP8_WIDTH * CHIP8_WINDOW_SIZE_MULTIPLIER, 
        CHIP8_HEIGHT * CHIP8_WINDOW_SIZE_MULTIPLIER, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET); // Render window

    while(1) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: // Checking if user clicked the exit button
                    goto out;
                    break;
                
                case SDL_KEYDOWN: // Checking if a key has been pressed
                    {char key = event.key.keysym.sym;
                    int vkey = chip8KeyboardMap(&chip8.keyboard, key);
                    if(vkey != -1) chip8KeyDown(&chip8.keyboard, vkey);}
                    break;

                case SDL_KEYUP: // Checking if a key has been released
                    {char key = event.key.keysym.sym;
                    int vkey = chip8KeyboardMap(&chip8.keyboard, key);
                    if(vkey != -1) chip8KeyUp(&chip8.keyboard, vkey);}
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        // Drawing pixels onto the screen
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
        // Setting game speed
        if(chip8.registers.delayTimer > 0) {
            Sleep(5);
            chip8.registers.delayTimer -= 1;
        }
        // Setting beep duration
        if(chip8.registers.soundTimer > 0) {
            Beep(2000, 10 * chip8.registers.soundTimer);
            chip8.registers.soundTimer = 0;
        }
        // Executes opcodes from memory
        unsigned short opcode = chip8MemoryGetShort(&chip8.memory, chip8.registers.programCounter);
        chip8.registers.programCounter += 2;
        chip8Exec(&chip8, opcode);
    }
out:
    SDL_DestroyWindow(window); // Closes window when button is clicked
    return 0;
}